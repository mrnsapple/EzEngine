#include <gtest/gtest.h>
#include "DoubleBufferAllocator.hpp"
#include <iostream>

using namespace ez;

TEST(TEST_DOUBLEBUFFER_ALLOCATOR_OBJECT, InputChar) {
	void *dee =   (uint8_t*)malloc(33 * sizeof(uint8_t));
	free(dee);
	
	DoubleBufferedAllocator a(100);
	for (int i = 0; i < 1; i++) {
		a.Reset();

		char *str = (char *) a.Allocate(20);
		EXPECT_NE(nullptr, str);
		str = (char *) a.Allocate(30);
		EXPECT_NE(nullptr, str);
		str = (char *) a.Allocate(80);
		EXPECT_EQ(nullptr, str);
		a.SwapBuffers();
		a.ZeroMem();
		//Use content stored in previous frame

	}
	a.Release();
	a.SwapBuffers();
	a.Release();


}