/**
 * @file "Core/ThreadPool/Public/ThreadMechanics.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_THREADPARALLELMECHANICS_HPP
#define ENGINE_THREADPARALLELMECHANICS_HPP

#include <functional>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <deque>
#include <stack>
#include "ThreadMechanics.hpp"
#include "Thread.hpp"

namespace ez::details
{
	class Thread;

	class ThreadParallelMechanics : public ThreadMechanics
	{
	public:
#ifdef ThreadParallelMechanics_TEST
		friend class TEST_ThreadParallelMechanics;
#endif
		friend class Thread;
		ThreadParallelMechanics(unsigned int hint);
#ifdef ThreadParallelMechanics_TEST
		ThreadParallelMechanics(unsigned int hint, unsigned int thread_spawn) : _size_threads(0), _interruptor(false),
	_threads(), _hint(hint), _size_inactive_threads(0)
		{
			for (auto i = 0; i < thread_spawn; i++)
				this->_threads.emplace_back(this);
			this->_size_threads.fetch_add(thread_spawn);
		}
#endif
		~ThreadParallelMechanics();

		template <typename F>
		void push(F &&f);

		bool	mustThreadStop() const noexcept final;
		void 	signalTaskEndForThread() final;

		void 	run();

		std::size_t 	getSizeTasks() noexcept;
		std::size_t		getSizeOfInactiveThread() const noexcept;
		std::size_t		getSizeOfThreads() const noexcept;

		std::function<void()>	getTask() final;
		bool					mustIQuit() noexcept final;
		bool 					isThreadEliminated() noexcept;
		bool					checkAllThreadInvalid() noexcept;
		void					clearTask();

	private:
		void 					stop();

		std::deque<Thread>			_threads;
		std::atomic<std::size_t>	_size_threads;
		std::atomic<std::size_t>	_size_inactive_threads;
		std::condition_variable		_sleeping_threads;
		std::mutex					_mtx_sleeping_threads;
		std::deque<std::function<void()>>	_tasks;
		std::atomic<bool>			_interruptor;
		std::mutex					_mtx_member_access;
		std::atomic<bool>			_thread_elimination {false};
		std::stack<std::thread::id>	_thread_eliminated;
		const unsigned int			_hint;
	};
}

#include "../Private/ThreadParallelMechanics.tpp"

#endif //ENGINE_THREADPARALLELMECHANICS_HPP
