/**
 * @file "__tests__/Core/Log/time.cpp"
 *
 * @author Alexandre Chetafi
**/

#include <gtest/gtest.h>
#include "Time.hpp"

using namespace ez;

TEST(Time, EvaluateStorage)
{
	auto time = Time::seconds(8);
	ASSERT_EQ(time.getMicroseconds(), 8000000);
	ASSERT_EQ(time.getSeconds(), 8);
}

TEST(Time, EvaluateConstOperator)
{
	auto time_a = Time::seconds(1);
	auto time_b = Time::seconds(2);

	EXPECT_TRUE(time_a < time_b);
	EXPECT_TRUE(time_a <= time_b);
	EXPECT_FALSE(time_a > time_b);
	EXPECT_FALSE(time_a >= time_b);
	EXPECT_FALSE(time_a == time_b);
	EXPECT_TRUE(time_a != time_b);

	auto time_c = Time::seconds(4);
	auto time_d = Time::seconds(4);

	EXPECT_FALSE(time_c < time_d);
	EXPECT_FALSE(time_c > time_d);
	EXPECT_TRUE(time_c <= time_d);
	EXPECT_TRUE(time_c >= time_d);
	EXPECT_TRUE(time_c == time_d);
	EXPECT_FALSE(time_c != time_d);
}

TEST(Time, EvaluateTimeNativeInt)
{
	auto time = Time::seconds(8);
/*
	auto add_a = Time + 4;
	auto add_b = 4 + Time;
	EXPECT_EQ(add_a, add_b);
*/
	auto mult_a = time * 4;
	auto mult_b = 4 * time;
	EXPECT_EQ(mult_a, mult_b);
	EXPECT_EQ(mult_a.getSeconds(), 32);

	auto mult_em = time;
	mult_em *= 4;
	EXPECT_EQ(mult_a.getSeconds(), mult_em.getSeconds());

	auto div_a = time / 2;
	EXPECT_EQ(div_a.getSeconds(), 4);
	EXPECT_NE(div_a, Time(0));

	auto div_em = time;
	div_em /= 2;
	EXPECT_EQ(div_a.getSeconds(), div_em.getSeconds());
}

TEST(Time, EvaluateNativeFloat)
{
	auto time = Time::seconds(8);
	auto time_red = Time::seconds(4);

	auto mult_a_float = time * (float) 0.5f;
	EXPECT_EQ(mult_a_float.getSeconds(), time_red.getSeconds());

	auto time_mult = time;
	time_mult *= 0.5f;
	EXPECT_EQ(time_mult.getSeconds(), time_red.getSeconds());

	auto div_a_float = time_red / 0.5f;
	EXPECT_EQ(div_a_float, time);

	auto time_div = time_red;
	time_div /= 0.5f;
	EXPECT_EQ(div_a_float.getSeconds(), time_div.getSeconds());
}

TEST(Time, EvaluateTimeOperation)
{
	auto time = Time::seconds(9);

	auto time_test = - time;
	EXPECT_EQ(time_test.getSeconds(), -9);

	auto time_add = Time::seconds(4);
	auto time_sub = Time::seconds(53);

	time_test -= time_sub;
	EXPECT_EQ(time_test.getSeconds(), -62);

	time_test += time_add;
	EXPECT_EQ(time_test.getSeconds(), -58);

	auto xa = time_test + time_sub;
	EXPECT_EQ(xa.getSeconds(), -5);
	auto xb = time_test - time_add;
	EXPECT_EQ(xb.getSeconds(), -62);
}