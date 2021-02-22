/**
 * @file "Core/ThreadPool/Private/Thread.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "ThreadMechanics.hpp"
#include "Thread.hpp"
#include <thread>

namespace ez::details
{
	Thread::Thread(ThreadMechanics *owner) : _owners(owner), _task(nullptr)
	{
		auto _func = [self = this] {
			while (!self->getStopFromOwner() && self->isThreadValid()) {
				try {
					while (self->isTaskAvailable()) {
						self->_task();
						self->_task = nullptr;
					}
					self->signalOwnerEndOfTask();
				} catch (std::exception &e) {
					std::cerr << "Thread crash, courtesy of thread pool" << std::endl;;
					std::cerr << e.what() << std::endl;
					std::exit(-1);
				}
			}
			self->forceStop();
		};
		_t = std::thread(_func);
	}
	
	Thread::Thread(Thread &&other) noexcept
	{
		std::unique_lock<std::mutex>	_lock(other._mutex);
		/* CRITICAL SECTION */
		this->_task = std::move(other._task);
		this->_t = std::move(other._t);
		this->_owners = other._owners;

		other.stop();
		other._task = nullptr;
	}
	
	Thread &Thread::operator=(Thread &&other) noexcept
	{
		this->stop();
		other.stop();
		std::scoped_lock	_lock(other._mutex, this->_mutex);
		/* CRITICAL SECTION */
		this->_task = other._task;
		this->_t = std::move(other._t);
		this->_owners = other._owners;

		other.stop();
		other._task = nullptr;
		return *this;
	}
	
	Thread::~Thread()
	{
		this->stop();
	}
	
	std::thread::id Thread::getId() const noexcept
	{
		return this->_t.get_id();
	}
	
	void Thread::stop()
	{
		if (this->_t.joinable())
			this->_t.join();
	}
	
	void Thread::signalOwnerEndOfTask()
	{
		this->_owners->signalTaskEndForThread();
	}

	bool Thread::getStopFromOwner() const
	{
		return this->_owners->mustThreadStop();
	}

	bool Thread::isTaskAvailable()
	{
		this->_task = this->_owners->getTask();
		return this->_task != nullptr;
	}

	void Thread::forceStop()
	{
		this->_thread_invalid.store(true);
	}

	bool Thread::isThreadValid()
	{
		if (this->_owners->mustIQuit())
			this->forceStop();
		return !this->_thread_invalid.load(std::memory_order_relaxed);
	}
}