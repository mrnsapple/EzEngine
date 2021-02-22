/**
 * @file "Core/EventTests/EventTest.cpp"
 *
 * @author Oriol Manzano
 */

#include <gtest/gtest.h>
#include "Actor.hpp"


using namespace ez;

TEST(EventTest, DispatchEvent) {

	SomeActor _obj;
	Event _e;
	Variant _var = {Variant::TYPE_DOUBLE, 44};
	_var._type = Variant::TYPE_INT;
	_var._asInt = 3;
	_e._type = EventType::MouseButtonPressed;
	_e._category = EventCategory::EventCategoryKeyboard;
	_e._variants.push_back(_var);
	EXPECT_EQ(_e._variants.size(), 1);
	_obj.OnEvent(_e);
	EXPECT_EQ(_obj.getAtack(), 44);
	_obj.OnEvent(_e);
	EXPECT_EQ(_obj.getAtack(), 88);

	_e._type = EventType::KeyPressed;
	_obj.OnEvent(_e);
	EXPECT_EQ(_obj.getAtack(), 165);
	_obj.OnEvent(_e);
	EXPECT_EQ(_obj.getAtack(), 165);
}


TEST(MouseEventTest, DispatchEvent) {

	SomeActor _obj;
	MouseEvent _e(29,11);


	_obj.OnEvent(_e);
	EXPECT_EQ(_obj.getAtack(), 44);
	_obj.OnEvent(_e);
	EXPECT_EQ(_obj.getAtack(), 44);
	_obj.OnEvent(_e);	
	EXPECT_EQ(_obj.getAtack(), 44);
}