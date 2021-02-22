/**
 * @file "__tests__/Core/Log/clock.cpp"
 *
 * @author Alexandre Chetafi
**/

#include <gtest/gtest.h>
#include "Clock.hpp"
#include <thread>
#include <chrono>

using namespace ez;

TEST(Clock, GenericTest)
{
	Clock c;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	EXPECT_NE(c.getTimeElapsed().getMicroseconds(), 0);
	auto round = c.restart();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	EXPECT_NE(round.getMicroseconds(), 0);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	EXPECT_NE(c.getTimeElapsed().getMicroseconds(), 0);
}