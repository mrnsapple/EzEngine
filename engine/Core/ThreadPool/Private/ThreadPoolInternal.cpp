/**
 * @file "Core/ThreadPool/Private/ThreadPoolInternal.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "ThreadPoolInternal.hpp"

namespace ez::details
{
	std::unique_ptr<ThreadPoolInternal> ThreadPoolInternal::create(int hint)
	{
		return std::make_unique<ThreadPoolInternal>(hint);
	}

	ThreadPoolInternal::ThreadPoolInternal(int hint) : _post_workers(hint)
	{}

	void ThreadPoolInternal::run()
	{
		this->_stop_asked.store(false, std::memory_order_relaxed);
		do {
			if (this->_stop_asked.load())
				break;
			this->_dispatch_worker.run();
			this->_post_workers.run();
			std::this_thread::sleep_for(std::chrono::microseconds(800));
		} while (this->isThereAnyTasksLeft() || this->_maintain_run.load(std::memory_order_relaxed));
	}

	bool ThreadPoolInternal::isThereAnyTasksLeft() noexcept
	{
		return this->_post_workers.getSizeOfThreads() == this->_post_workers.getSizeOfInactiveThread() &&
		this->_dispatch_worker.numberOfTasksRemaining();
	}
}