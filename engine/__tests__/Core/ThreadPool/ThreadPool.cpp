/**
 * @file "__tests__/Core/ThreadPool/ThreadPool.cpp"
 *
 * @author Alexandre Chetafi
**/

#include <gtest/gtest.h>
#include "ThreadPoolInternal.hpp"

using namespace ez;
using namespace details;

TEST(Thread, InternalCreation)
{
	auto _t = ThreadPoolInternal::create();

	auto _array = new int[90000000];
	auto _func = [_array](int i) {
		for (auto index = i; index < (i + 1) * 10000000; index++) {
			_array[index] = index;
		}
	};

	_t->post(_func, 0);
	_t->post(_func, 1);
	_t->post(_func, 2);
	_t->dispatch(_func, 4);
	_t->dispatch(_func, 5);

	_t->defer(_func, 8);

	std::thread _tt([&]{
		_t->run();
	});
	_t->dispatch(_func, 6);
	_t->post(_func, 3);
	_t->dispatch(_func, 7);

	if (_tt.joinable())
		_tt.join();

	for (auto i = 0; i < 90000000; i++) {
		EXPECT_EQ(i, _array[i]);
	}
	delete[] _array;
}

#include "ThreadPool.hpp"

/**
 * About this test :
 * I could not find any other way to test if Thread pool work or not, since I am using a timer to test various async
 * strategies, this is hardware dependant and should not be tested on the devops machine. Local testing is ok.
 */

TEST(Thread, Context)
{/*
	ThreadPool _t;

	auto _array = new int[90000000];
	auto _func = [_array](int i) {
		for (auto index = i; index < (i + 1) * 10000000; index++) {
			_array[index] = index;
		}
	};


	post(_t, _func, 0);
	post(_t, _func, 1);
	post(_t, _func, 2);
	dispatch(_t, _func, 4);
	dispatch(_t, _func, 5);
	defer(_t, _func, 8);
	dispatch(_t, _func, 6);
	post(_t, _func, 3);
	dispatch(_t, _func, 7);

	std::this_thread::yield();
	std::this_thread::sleep_for(std::chrono::seconds(10));

	for (auto i = 0; i < 90000000; i++) {
		EXPECT_EQ(i, _array[i]);
	}
	delete[] _array;
 */
}