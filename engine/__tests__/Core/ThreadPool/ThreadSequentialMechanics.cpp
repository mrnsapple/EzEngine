/**
 * @file "ThreadSequentialMechanics.cpp"
 *
 * @author Alexandre Chetafi
**/

#define ThreadSequentialMechanics_TEST

#include <gtest/gtest.h>
#include "Thread.hpp"
#include "ThreadSequentialMechanics.hpp"

namespace ez::details
{
	class TEST_ThreadSequentialMechanics
	{
	public:
		static void stop(ThreadSequentialMechanics &x) {
			x.stop();
		}
	};
}

using namespace ez;
using namespace details;

TEST(Thread, SequentialMechanismTestEmpty)
{
	ThreadSequentialMechanics	p;

	EXPECT_EQ(p.numberOfTasksRemaining(), 0);
}

TEST(Thread, SequentialMechanismTestPushEmptyTask)
{
	ThreadSequentialMechanics	p;
	auto func_a = []{};
	auto func_b = []{};

	p.push(func_a);
	p.push(func_b);
	p.run();
	ASSERT_EQ(p.numberOfTasksRemaining(), 0);
}

TEST(Thread, SequentialMechanismTestPushTask)
{
	ThreadSequentialMechanics	p;
	int *x = new int[300000];
	auto func_a = [&]{
		for (auto i = 0; i < 100000; i++) {
			x[i] = i;
		}
	};
	auto func_b = [&]{
		for (auto i = 100000; i < 200000; i++) {
			x[i] = i;
		}
	};
	auto func_c = [&]{
		for (auto i = 200000; i < 300000; i++) {
			x[i] = i;
		}
	};

	p.push(func_a);
	p.push(func_b);
	p.push(func_c);
	p.run();
	for (auto i = 0; i < 300000; i++) {
		ASSERT_EQ(i, x[i]);
	}
	delete[] x;
}

#include <cstring>

TEST(Thread, SequentialMechanismTestSoftStop)
{
	auto start = std::chrono::steady_clock::now();
	ThreadSequentialMechanics	p;
	int *x = new int[300000];
	std::memset(x, 0, sizeof(int) * 300000);
	auto func_a = [&]{
		for (auto i = 0; i < 100000; i++) {
			x[i] = i;
		}
	};
	auto func_b = [&]{
		for (auto i = 100000; i < 200000; i++) {
			x[i] = i;
		}
	};
	auto func_c = [&]{
		for (auto i = 200000; i < 300000; i++) {
			x[i] = i;
		}
	};

	TEST_ThreadSequentialMechanics::stop(p);
	p.push(func_a);
	p.push(func_b);
	p.push(func_c);
	p.run();
	for (auto i = 0; i < 300000; i++) {
		/* The thread fail to not initialize any tasks, meaning the sequential tasks are leaking */
		ASSERT_EQ(x[i], 0);
	}
	delete[] x;
	EXPECT_EQ(p.numberOfTasksRemaining(), 0);
}