/**
 * @file "Core/Time/Public/Clock.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_CLOCK_HPP
#define ENGINE_CLOCK_HPP

#include "Time.hpp"

namespace ez
{
	class Time;

	class Clock
	{
	public:
		Clock();
		~Clock() = default;
		Time	getTimeElapsed();
		Time	restart();
	private:
		Time	_start;
	};
}

#endif //ENGINE_CLOCK_HPP
