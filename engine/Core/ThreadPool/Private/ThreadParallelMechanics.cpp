/**
 * @file "Core/ThreadPool/Private/ThreadParallelMechanics.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "ThreadParallelMechanics.hpp"
#include "Thread.hpp"
#include <iostream>

namespace ez::details
{
	ThreadParallelMechanics::ThreadParallelMechanics(unsigned int hint) : _size_threads(0), _interruptor(false),
	_threads(), _hint(hint), _size_inactive_threads(0)
	{
		for (auto i = 0; i < hint; i++)
			this->_threads.emplace_back(this);
		this->_size_threads.fetch_add(hint);
	}

	ThreadParallelMechanics::~ThreadParallelMechanics()
	{
		this->stop();
	}

	bool ThreadParallelMechanics::mustThreadStop() const noexcept
	{
		return this->_interruptor.load(std::memory_order_relaxed);
	}

	void ThreadParallelMechanics::signalTaskEndForThread()
	{
		if (this->mustThreadStop())
			return;
		this->_size_inactive_threads.fetch_add(1, std::memory_order_relaxed);
		std::unique_lock<std::mutex>	_lock(this->_mtx_sleeping_threads);
		this->_sleeping_threads.wait(_lock);
		this->_size_inactive_threads.fetch_sub(1, std::memory_order_relaxed);
	}

	void ThreadParallelMechanics::run()
	{
		while (this->getSizeTasks()) {
			if (!this->getSizeOfInactiveThread()) {
				std::unique_lock<std::mutex>	_lock(this->_mtx_member_access);
				this->_threads.emplace_back(this);
			}
			this->_sleeping_threads.notify_one();
		}
		if (this->getSizeOfInactiveThread() > (this->_hint + 2 + this->getSizeTasks())) {
			this->_thread_elimination.store(true, std::memory_order_relaxed);
			this->_sleeping_threads.notify_one();
		} else if (this->isThreadEliminated()) {
			std::unique_lock<std::mutex>	_lock(this->_mtx_member_access);
			auto &_tmp_id = this->_thread_eliminated.top();
			auto it = std::find_if(this->_threads.begin(), this->_threads.end(), [_tmp_id](const Thread &thread) {
				return thread.getId() == _tmp_id;
			});
			if (it != this->_threads.end()) {
				this->_threads.erase(it);
			}
			this->_thread_eliminated.pop();
		}
	}

	std::size_t ThreadParallelMechanics::getSizeOfInactiveThread() const noexcept
	{
		return this->_size_inactive_threads.load(std::memory_order_relaxed);
	}

	std::size_t ThreadParallelMechanics::getSizeOfThreads() const noexcept
	{
		return this->_size_threads.load(std::memory_order_relaxed);
	}

	std::size_t ThreadParallelMechanics::getSizeTasks() noexcept
	{
		std::unique_lock<std::mutex>	_lock(this->_mtx_member_access);
		return this->_tasks.size();
	}

	std::function<void ()> ThreadParallelMechanics::getTask()
	{
		std::unique_lock<std::mutex>	_lock(this->_mtx_member_access);
		if (this->_tasks.empty())
			return nullptr;
		auto func = this->_tasks.front();
		this->_tasks.pop_front();
		return func;
	}

	void ThreadParallelMechanics::stop()
	{
		this->_interruptor.store(true, std::memory_order_relaxed);
		this->clearTask();
		do {
			this->_sleeping_threads.notify_all();
			std::this_thread::sleep_for(std::chrono::microseconds(500));
		} while(this->checkAllThreadInvalid());
	}

	bool ThreadParallelMechanics::mustIQuit() noexcept
	{
		if (this->_thread_elimination.load(std::memory_order_relaxed)) {
			this->_thread_elimination.store(false, std::memory_order_relaxed);
			std::unique_lock<std::mutex>	_lock(this->_mtx_member_access);
			this->_thread_eliminated.push(std::this_thread::get_id());
			return true;
		}
		return false;
	}

	bool ThreadParallelMechanics::isThreadEliminated() noexcept
	{
		std::unique_lock<std::mutex>	_lock(this->_mtx_member_access);
		return !this->_thread_eliminated.empty();
	}

	bool ThreadParallelMechanics::checkAllThreadInvalid() noexcept
	{
		for (auto &thread : this->_threads) {
			if (!thread.isThreadValid())
				return false;
		}
		return true;
	}

	void ThreadParallelMechanics::clearTask()
	{
		std::unique_lock<std::mutex>	_lock(this->_mtx_member_access);
		this->_tasks.clear();
		this->_tasks.shrink_to_fit();
	}
}
