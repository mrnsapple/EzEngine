/**
 * @file "Core/ThreadPool/Public/ThreadSequentialMechanics.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_THREADSEQUENTIALMECHANICS_HPP
#define ENGINE_THREADSEQUENTIALMECHANICS_HPP

#include "ThreadMechanics.hpp"
#include <mutex>
#include <condition_variable>
#include "Thread.hpp"

namespace ez::details
{
	class Thread;

	class ThreadSequentialMechanics : public ThreadMechanics
	{
	public:
#ifdef ThreadSequentialMechanics_TEST
		friend class TEST_ThreadSequentialMechanics;
#endif
		friend class Thread;
		ThreadSequentialMechanics();
		~ThreadSequentialMechanics();

		template <typename F>
		void 		push(F &&f);
		unsigned int numberOfTasksRemaining() noexcept;

		bool 	mustThreadStop() const noexcept final;
		void	signalTaskEndForThread() final;

		void 	run();

	private:
		void					stop();
		std::size_t				getTasksSize();
		std::function<void()>	getTask() final;
		bool					mustIQuit() noexcept final;

		std::deque<std::function<void()>>	_tasks;
		std::mutex							_mutex;
		std::mutex							_waiting_thread;
		std::condition_variable				_sleeping_thread;
		std::unique_ptr<Thread>				_thread;
		std::atomic<bool>					_inactive_thread;
		std::atomic<bool>					_interruptor;
	};
}

#include "../Private/ThreadSequentialMechanics.tpp"

#endif //ENGINE_THREADSEQUENTIALMECHANICS_HPP
