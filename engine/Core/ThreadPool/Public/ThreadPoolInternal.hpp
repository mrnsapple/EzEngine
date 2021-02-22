/**
 * @file "Core/ThreadPool/Public/ThreadPool.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_THREADPOOLINTERNAL_HPP
#define ENGINE_THREADPOOLINTERNAL_HPP

#include <memory>
#include "Singleton.hpp"
#include "ThreadSequentialMechanics.hpp"
#include "ThreadParallelMechanics.hpp"

namespace ez::details
{
	class ThreadParallelMechanics;
	class ThreadSequentialMechanics;

	class ThreadPoolInternal : public Singleton<ThreadPoolInternal>
	{
	public:
		/**
		 * Create the singleton pointer with hint
		 * @return A unique pointer on ThreadPool
		 */

		static std::unique_ptr<ThreadPoolInternal>		create(int hint = std::thread::hardware_concurrency());

		/**
		 * Start the threadPool with a hint on a number of thread
		 * @param hint Hint for the number of thread
		 */

		ThreadPoolInternal(int hint = std::thread::hardware_concurrency());

		/**
		 *
		 */

		void run();

		/**
		 * This method will send a task to a thead, as soon as a place is available
		 * @tparam F
		 * @param f
		 */

		template <typename F, typename ...Args>
		void defer(F &&f, Args&&... args);

		/**
		 * Will spawn a new thread if called from a different thread. (post)
		 * @tparam F A void() Function type
		 * @param f A function
		 */

		template <typename F, typename ...Args>
		void post(F &&f, Args&&... args);

		/**
		 * Execute function in sequential order, and won't spawn new thread to run the method
		 * @tparam F A void() Function type
		 * @param f A function
		 */

		template <typename F, typename ...Args>
		void dispatch(F &&f, Args&&... args);

	private:
		bool isThereAnyTasksLeft() noexcept;

	private:
		ThreadSequentialMechanics	_dispatch_worker;
		ThreadParallelMechanics		_post_workers;
		std::atomic<int>			_maintain_run {0};
		std::atomic<bool>			_stop_asked {false};
	};
}

#include "../Private/ThreadPoolInternal.tpp"

#endif //ENGINE_THREADPOOLINTERNAL_HPP
