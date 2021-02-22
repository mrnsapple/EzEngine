/**
 * @file "Core/ThreadPool/Public/ThreadPool.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_THREADPOOL_HPP
#define ENGINE_THREADPOOL_HPP

#include <memory>
#include "ThreadPoolInternal.hpp"

namespace ez
{
	namespace details {
		class ThreadPoolInternal;
	}

	/**
	 * @brief The thread pool manager
	 *
	 * This class is supporting the manager to get pool context. It hides the run() method, to make it never quit, and is
	 * constantly aware of that run() must be executed unless the destructor of the pool is called.
	 */

	class ThreadPool
	{
	public:
		ThreadPool();
		~ThreadPool();

		details::ThreadPoolInternal	*getThreadPool();

	private:
		void		run_once();
		std::unique_ptr<details::ThreadPoolInternal>	_pool;
	};

	/**
	 * @brief Create a new thread for the task pass to post
	 * The post function always create a new thread to manage the task passed into. All return is always ignored, therefore
	 * only use void return type. As for the exception, the exception are not catch but, will make the program crash from the
	 * Thread itself. It has be designed this way to prevent unknown behavior if execution was ensured to continue inside the
	 * thread. There is no guarantee on the order of tasks' executions.
	 *
	 * @tparam F 	Any function that is void (event if not void, the result is not saved)
	 * @tparam Args Any type of arguments of the function F
	 * @param context Context to where to send the task
	 * @param f		Function f, which can be anything, even a lambda
	 * @param args	The arguments of the function f, if there is such thing
	 */
	template <typename F, typename ...Args>
	void post(ThreadPool &context, F &&f, Args&&... args);

	/**
	 * @brief Enqueue new thread runs in sequential order (from run).
	 * The dispatch function execute the task asynchronously, but never create a new thread. The underlying thread only
	 * execute tasks given without order, when the previous task has already finished. If a task is blocking the thread,
	 * then the whole thread will be blocked, and there is no hard release implemented. Like post, exception crash the program
	 * and return value are ignored.
	 *
	 * @tparam F 	Any function that is void (event if not void, the result is not saved)
	 * @tparam Args Any type of arguments of the function F
	 * @param context Context to where to send the task
	 * @param f		Function f, which can be anything, even a lambda
	 * @param args	The arguments of the function f, if there is such thing
	 */
	template <typename F, typename ...Args>
	void dispatch(ThreadPool &context, F &&f, Args&&... args);

	/**
	 * @brief Take the function and internally choose between a post or dispatch
	 * The defer function send a hint to the context, and the context will decide which strategies to use on the actual task.
	 * Like post and defer, exception will crash the program, return value are always ignored. There is no guarantee on the
	 * order of tasks' execution
	 *
	 * @tparam F 	Any function that is void (event if not void, the result is not saved)
	 * @tparam Args Any type of arguments of the function F
	 * @param context Context to where to send the task
	 * @param f		Function f, which can be anything, even a lambda
	 * @param args	The arguments of the function f, if there is such thing
	 */
	template <typename F, typename ...Args>
	void defer(ThreadPool &context, F &&f, Args&&... args);
}

#include "../Private/ThreadPool.tpp"

#endif //ENGINE_THREADPOOL_HPP
