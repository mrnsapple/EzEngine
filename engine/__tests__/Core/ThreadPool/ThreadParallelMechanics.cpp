/**
 * @file "ThreadParallelMechanics.cpp"
 *
 * @author Alexandre Chetafi
**/

#define ThreadParallelMechanics_TEST

#include <gtest/gtest.h>
#include "Thread.hpp"
#include "ThreadParallelMechanics.hpp"

namespace ez::details
{
	class TEST_ThreadParallelMechanics
	{
	public:
		static void stop(ThreadParallelMechanics &x) {
			x.stop();
		}
	};
}

using namespace ez;
using namespace details;

/**
 * This test should not be used.
 */

TEST(Thread, ParallelMechanismTestEmpty)
{
	/*
	ThreadParallelMechanics p(4);

	EXPECT_EQ(p.getSizeOfThreads(), 4);
	EXPECT_EQ(p.getSizeOfInactiveThread(), 4);
	EXPECT_EQ(p.getSizeTasks(), 0);

	p.run();

	EXPECT_EQ(p.getSizeOfInactiveThread(), 4);
	EXPECT_EQ(p.getSizeTasks(), 0);
	EXPECT_EQ(p.getSizeOfThreads(), 4);

	TEST_ThreadParallelMechanics::stop(p);

	ThreadParallelMechanics	q(4, 15);

	EXPECT_EQ(q.getSizeOfThreads(), 15);
	EXPECT_EQ(q.getSizeOfInactiveThread(), 15);
	EXPECT_EQ(q.getSizeTasks(), 0);

	q.run();

	EXPECT_EQ(q.getSizeOfInactiveThread(), 14);
	EXPECT_EQ(q.getSizeTasks(), 0);
	EXPECT_EQ(q.getSizeOfThreads(), 15);
*/
}

#include <cstring>

TEST(Thread, ParallelMechanismTestAccess)
{
	ThreadParallelMechanics p(1);
	static int *x = new int[8000000];
	std::memset(x, 0, sizeof(int) * 8000000);
	auto func = [&] (int id) {
		for (auto i = id * 1000000; i < (id + 1) * 1000000; i++) {
			x[i] = i;
		}
	};
	auto test_0 = [func]{ func(0); };
	auto test_1 = [func]{ func(1); };
	auto test_2 = [func]{ func(2); };
	auto test_3 = [func]{ func(3); };
	auto test_4 = [func]{ func(4); };
	auto test_5 = [func]{ func(5); };
	auto test_6 = [func]{ func(6); };
	auto test_7 = [func]{ func(7); };

	p.push(test_0);
	p.push(test_1);
	p.push(test_2);
	p.push(test_3);
	p.push(test_4);
	p.push(test_5);
	p.push(test_6);
	p.push(test_7);
	p.run();

	EXPECT_EQ(p.getSizeTasks(), 0);
	while (p.getSizeOfInactiveThread() < 0 && p.getSizeTasks() == 0)
		std::this_thread::sleep_for(std::chrono::seconds(1)); //TODO mettre un timeout
	for (auto i = 0; i < 400000; i++) {
		EXPECT_EQ(x[i], i);
	}
	delete[] x;
}