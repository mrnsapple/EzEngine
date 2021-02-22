#include <gtest/gtest.h>
#include "StackAllocator.hpp"
#include <iostream>

using namespace ez;

TEST(TEST_STACK_ALLOCATOR_OBJECT, InputChar) {
	StackAllocator *a = new StackAllocator(100);
	void *str = (void *) a->Allocate(20);
	std::cout <<"the str:" << (std::size_t)str<<std::endl;
	EXPECT_NE(nullptr, str);
	str = (char *) a->Allocate(30);
	std::cout <<"the str:" << (std::size_t)str<<std::endl;

	EXPECT_NE(nullptr, str);
	str = (char *) a->Allocate(80);
	EXPECT_EQ(nullptr, str);
	a->Release();


}
TEST(TEST_STACK_ALLOCATOR_USE_RESET, InputChar) {
	StackAllocator *a = new StackAllocator(100);
	void *str = (void *) a->Allocate(20);
	std::cout <<"the str:" << (std::size_t)str<<std::endl;
	EXPECT_NE(nullptr, str);
	str = (char *) a->Allocate(30);
	std::cout <<"the str:" << (std::size_t)str<<std::endl;

	EXPECT_NE(nullptr, str);
	str = (char *) a->Allocate(80);
	EXPECT_EQ(nullptr, str);
	a->Reset();

	str = (char *) a->Allocate(20);
	str = (char *) a->Allocate(30);

	EXPECT_NE(nullptr, str);

	a->Release();

}