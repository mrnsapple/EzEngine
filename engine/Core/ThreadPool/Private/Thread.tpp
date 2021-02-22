/**
 * @file "Core/ThreadPool/Private/Thread.tpp"
 *
 * @author Alexandre Chetafi
**/


#ifndef ENGINE_THREAD_CPP
#define ENGINE_THREAD_CPP

#include "ThreadMechanics.hpp"
#include <iostream>

namespace ez::details
{
	template <typename F>
	void Thread::doTask(F &&f)
	{
		std::unique_lock<std::mutex>	_lock(this->_mutex);
		/* CRITICAL SECTION */
		this->_task = f;
	}
}

#endif //ENGINE_THREAD_CPP