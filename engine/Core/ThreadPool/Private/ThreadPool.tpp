/**
 * @file "Core/ThreadPool/Private/ThreadPool.tpp"
 *
 * @author Alexandre Chetafi
**/

#include "ThreadPool_is_not_available.hpp"
#include "ThreadPoolInternal.hpp"

namespace ez
{
	template <typename F, typename ...Args>
	void post(ThreadPool &context, F &&f, Args&&... args)
	{
		if (context.getThreadPool()) {
			context.getThreadPool()->post(f, args...);
			return;
		}
		throw ThreadPool_is_not_available("", __FILE__, __LINE__);
	}

	template <typename F, typename ...Args>
	void dispatch(ThreadPool &context, F &&f, Args&&... args)
	{
		if (context.getThreadPool()) {
			context.getThreadPool()->dispatch(f, std::move(args...));
			return;
		}
		throw ThreadPool_is_not_available("", __FILE__, __LINE__);
	}

	template <typename F, typename ...Args>
	void defer(ThreadPool &context, F &&f, Args&&... args)
	{
		if (context.getThreadPool()) {
			context.getThreadPool()->defer(f, std::move(args...));
			return;
		}
		throw ThreadPool_is_not_available("", __FILE__, __LINE__);
	}
}