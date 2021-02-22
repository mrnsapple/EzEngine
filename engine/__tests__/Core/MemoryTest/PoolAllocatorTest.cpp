#include <gtest/gtest.h>
#include "Actor.hpp"
#include "ChildActor.hpp"
#include "PoolManager.hpp"
using namespace ez;

//PoolManager *PoolManager::instance = nullptr;

TEST(TEST_POOL_ALLOCATOR_Actor, Constructor_not_null) {
	PoolManager *s = s->getNewInstance();
	Actor *Actors;
	Actors = new Actor();
	EXPECT_NE(nullptr, Actors);
}

TEST(TEST_POOL_ALLOCATOR_Actor, Realocate_Free_Memory) {
	PoolManager *s = s->getNewInstance();
	constexpr int arraySize = 10;
  	Actor *Actors[3];
	Actors[0] = new Actor();
	Actors[1] = new Actor();
	EXPECT_NE(nullptr, Actors[0]);
	EXPECT_NE(nullptr, Actors[0]);
	delete(Actors[0]);
	Actors[2] = new Actor();
	EXPECT_EQ(Actors[2], Actors[0]);
	EXPECT_NE(Actors[1], Actors[2]);
}

TEST(TEST_POOL_ALLOCATOR_Actor, Realocate_Freee_Memory) {
	PoolManager *s = s->getNewInstance();
	constexpr int arraySize = 10;
  	Actor *Actors[3];
	Actors[0] = new Actor();
	Actors[1] = new Actor();
	EXPECT_NE(nullptr, Actors[0]);
	EXPECT_NE(nullptr, Actors[0]);
	delete(Actors[0]);
	Actors[2] = new Actor();
	EXPECT_EQ(Actors[2], Actors[0]);
	EXPECT_NE(Actors[1], Actors[2]);
}
TEST(TEST_POOL_ALLOCATOR_Actor, AllocateMultiplePools) {
	PoolManager *s = s->getNewInstance();
	constexpr int arraySize = 10;
	Actor *Actors[arraySize];
	
	for (int i = 0; i < arraySize; ++i) {
		Actor *prev = new Actor();
		EXPECT_NE(nullptr, prev);
		if (i <= 7)
			EXPECT_EQ(1, s->getPoolNumber());
		else
			EXPECT_EQ(2, s->getPoolNumber());
		Actors[i] = prev;
	}
}

TEST(TEST_POOL_ALLOCATOR_Actor, AllocateMultiplePoolsDiferentSizes) {
	PoolManager *s = s->getNewInstance();
	constexpr int arraySize = 10;
	EXPECT_EQ(0, s->getPoolNumber());
	
	for (int i = 0; i < 3; ++i) {
		Actor *prev = new Actor();
		EXPECT_NE(nullptr, prev);
	}
	EXPECT_EQ(1, s->getPoolNumber());
	ChildActor *child = new ChildActor();
	EXPECT_EQ(2, s->getPoolNumber());

}
