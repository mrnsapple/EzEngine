//
// Created by phill on 02/11/2020.
//

#include "KeyBoardEvent.hpp"

namespace ez
{
	KeyboardEvent::KeyboardEvent(const EventType &type, Keyboard::key combination, Keyboard::key key, bool isPressed) : _key(key),
	 _combination(combination & Keyboard::ALT, combination & Keyboard::CTRL, combination & Keyboard::SHIFT, false) //TODO
	{
		_category = EventCategory::EventCategoryKeyboard;
		_type=type;
	}

	const EventCategory &KeyboardEvent::getCategory() const noexcept
	{
		return this->_category;
	}

	const Keyboard::key & KeyboardEvent::getKey() const noexcept
	{
		return this->_key;
	}

	const EventType & KeyboardEvent::getType() const noexcept
	{
		return this->_type;
	}

	const KeyboardEvent::combination &KeyboardEvent::getKeyCombination() const noexcept
	{
		return this->_combination;
	}
}