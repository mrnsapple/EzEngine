/**
 * @file "Core/ThreadPool/Public/ThreadMechanics.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_THREADMECHANICS_HPP
#define ENGINE_THREADMECHANICS_HPP

#include <thread>
#include <functional>

namespace ez::details
{
	class	Thread;

	class 	ThreadMechanics
	{
	public:
		friend class Thread;
		virtual bool 	mustThreadStop() const noexcept = 0;
		virtual void 	signalTaskEndForThread() = 0;

	protected:
		virtual std::function<void()>	getTask() = 0;
		virtual bool					mustIQuit() noexcept = 0;
	};
}

#endif //ENGINE_THREADMECHANICS_HPP
