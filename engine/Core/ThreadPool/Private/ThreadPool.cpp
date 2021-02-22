/**
 * @file "Core/ThreadPool/Private/ThreadPool.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "ThreadPool.hpp"

namespace ez
{
	/**
	 * The system is missing a guard and therefore such poorly crude technique is necessary
	 */

	namespace {
		std::thread _internal;
		std::atomic<bool>	_end_internal {false};
	}

	ThreadPool::ThreadPool() : _pool(details::ThreadPoolInternal::create())
	{
		this->run_once();
	}

	ThreadPool::~ThreadPool()
	{
		_end_internal.store(true, std::memory_order_relaxed);
		if (_internal.joinable())
			_internal.join();
	}

	details::ThreadPoolInternal *ThreadPool::getThreadPool()
	{
		return this->_pool.get();
	}

	void ThreadPool::run_once()
	{
		_internal = std::thread([this]{
			while (!_end_internal.load()) {
				this->_pool->run();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		});
	}
}