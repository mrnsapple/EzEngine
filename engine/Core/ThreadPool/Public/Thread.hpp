/**
 * @file "Core/ThreadPool/Public/Thread.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_THREAD_HPP
#define ENGINE_THREAD_HPP

#include <thread>
#include <functional>
#include <deque>
#include <atomic>
#include <mutex>

namespace ez::details
{
	class ThreadMechanics;
	/**
	 * This class is used as a Thread by the tread Pool or in a Thread Group. It is ill advise to make it work alone
	 * as some control from a manager may be necessary. The execution inside the thread is exception unsafe. It is
	 * therefore dependant on the implementation of the task with a std::error_code.
	 * @tparam Mechanics A scheme to own the thread and give the thread a possibility to stop itself
	 */

	class Thread
	{
	public:
		using threadID = std::thread::id;
		Thread	&operator=(Thread &) = delete;
		Thread(Thread &) = delete;
		friend class ThreadMechanics;
		friend class ThreadSequentialMechanics;

		/**
		 * The constructor, which actually build a function waiting inside the thread for new tasks.
		 */

		Thread(ThreadMechanics *owner);

		/**
		 * move constructor
		 */

		Thread(Thread &&) noexcept;

		/**
		 * Thread	&operator=(Thread &&);
		 * @return
		 */

		Thread	&operator=(Thread &&) noexcept;

		/**
		 * The destructor activate an interuptor to stop the actual thread working
		 */

		~Thread();

		/**
		 * The id of the underlying native thread
		 * @return std::thread::id type of information
		 */

		threadID 		getId() const noexcept;

		/**
		 * This is the method for pushing tasks in the thread as soon as the thread is available
		 * @tparam F Any Function return a void
		 * @tparam Args Any arguments
		 * @param function The Function
		 * @param arguments The arguments
		 */

		template <typename F>
		void 			doTask(F &&function);

		/**
		 * This function stop the thread from executing any more tasks. It is to note that the actual tasks will be waited
		 * upon, and all other task will not be executed. All task pass throught task() method will be ignored. This action
		 * is irreversible.
		 */

		void 			stop();

		/**
		 * Ask if the thread is still valid. If not, the underlying thread will stop
		 * @return True if valid
		 */

		bool 					isThreadValid();

	private:

		/**
		 * Signal to the owner tha thread will become inactive
		 */

		void					signalOwnerEndOfTask();

		/**
		 * Ask the owner if underlying thread's loop should continue
		 * @return True for a stop of the underlying thread
		 */

		bool 					getStopFromOwner() const;

		/**
		 * Check if a function has been stored into the member of this class by the owner
		 * @return False if nullptr
		 */

		bool 					isTaskAvailable();

		/**
		 * Asking to forcefully stop the thread and quitting the underlying thread's loop
		 */

		void 					forceStop();

	private:
		std::atomic<bool>		_thread_invalid {false};
		std::function<void()>	_task;
		std::thread				_t;
		std::mutex				_mutex;
		ThreadMechanics			*_owners;
	};
}

#include "../Private/Thread.tpp"

#endif //ENGINE_THREAD_HPP
