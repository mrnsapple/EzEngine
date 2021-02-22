/**
 * @file "Core/Time/Private/Clock.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "Clock.hpp"

#include <chrono>

namespace ez
{
	Clock::Clock() : _start(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
	{}

	Time Clock::getTimeElapsed()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - _start;
	}

	Time Clock::restart()
	{
		Time now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		Time elapsed = now - _start;
		_start = now;
		return elapsed;
	}
}