/**
 * @file "Core/ThreadPool/Private/ThreadSequentialMechanics.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "Thread.hpp"
#include "ThreadSequentialMechanics.hpp"

namespace ez::details
{
	ThreadSequentialMechanics::ThreadSequentialMechanics() : _inactive_thread(true), _interruptor(false)
	{
		this->_thread = std::make_unique<Thread>(this);
	}

	ThreadSequentialMechanics::~ThreadSequentialMechanics()
	{
		this->stop();
	}

	unsigned int ThreadSequentialMechanics::numberOfTasksRemaining() noexcept
	{
		std::unique_lock<std::mutex>		_lock(this->_mutex);
		return this->_tasks.size();
	}

	bool ThreadSequentialMechanics::mustThreadStop() const noexcept
	{
		return this->_interruptor.load(std::memory_order_relaxed);
	}

	void ThreadSequentialMechanics::signalTaskEndForThread()
	{
		if (this->mustThreadStop())
			return;
		this->_inactive_thread.store(true, std::memory_order_relaxed);
		std::unique_lock<std::mutex>			_lock(this->_waiting_thread);
		this->_sleeping_thread.wait(_lock);
		this->_inactive_thread.store(false, std::memory_order_relaxed);
	}

	void ThreadSequentialMechanics::run()
	{
		while (this->getTasksSize() && !this->mustThreadStop()) {
			this->_sleeping_thread.notify_all();
		}
		if (this->mustThreadStop()) {
			this->_tasks.clear();
		}
	}

	void ThreadSequentialMechanics::stop()
	{
		this->_interruptor.store(true);
		while (this->_thread->isThreadValid()) {
			this->_sleeping_thread.notify_all();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	std::size_t ThreadSequentialMechanics::getTasksSize()
	{
		std::unique_lock<std::mutex>	_lock(this->_mutex);
		return this->_tasks.size();
	}

	std::function<void ()> ThreadSequentialMechanics::getTask()
	{
		if (this->getTasksSize()) {
			std::unique_lock<std::mutex>	_lock(this->_mutex);
			auto func = this->_tasks.front();
			this->_tasks.pop_front();
			return func;
		}
		return nullptr;
	}

	bool ThreadSequentialMechanics::mustIQuit() noexcept
	{
		return false;
	}
}