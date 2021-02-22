#pragma once

#include "Event.hpp"
#ifndef MOUSEEVENT_H_
#define MOUSEEVENT_H_

namespace ez
{
	namespace Mouse {
		enum key {
			M_LEFT,
			M_RIGHT,
			M_CENTER,
			M_MOVE
		};
	}

	class MouseEvent : public Event
	{
	public:
		MouseEvent() { _category = EventCategory::EventCategoryMouse; };
		MouseEvent(int, int) {} // legacy code
		int getX() const { return _mouseX; }
		int getY() const { return _mouseY; }

	public:
		int 				_mouseX = 0, _mouseY = 0;
		Mouse::key 			_key;
		bool 				_isdbClick;
		struct combination {
			bool shift	{false};
			bool ctrl	{false};
		} 					_combination;
	};
}

#endif