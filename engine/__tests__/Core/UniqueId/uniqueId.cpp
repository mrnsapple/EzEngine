/**
 * @file "uniqueIdManager.hpp"
 *
 * @author Alexandre Chetafi
**/

#include <gtest/gtest.h>
#include "UniqueIDManager.hpp"
#include "UniqueID.hpp"

using namespace ez;

TEST(UniqueID, UniqueIDManager)
{
	static auto x = UniqueIDManager::create();
	UniqueID	a;
	UniqueID	b;
	UniqueID	c;

	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);
	EXPECT_TRUE(a.isValid());
	EXPECT_TRUE(b.isValid());

	auto value_b = b.getValue();
	auto z = std::move(b);
	EXPECT_EQ(z.getValue(), value_b);
	EXPECT_EQ(b.getValue(), 0);
	EXPECT_FALSE(b.isValid());
}