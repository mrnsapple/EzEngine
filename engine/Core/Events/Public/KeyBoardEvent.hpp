//
// Created by phill on 02/11/2020.
//

#ifndef ENGINE_KEYBOARDEVENT_HPP
#define ENGINE_KEYBOARDEVENT_HPP

#include "Event.hpp"

namespace ez
{
	namespace Keyboard {
		enum key : std::uint64_t {
			KEY_A = 0,
			KEY_B = 1,
			KEY_C = 2,
			KEY_D = 3,
			KEY_E = 4,
			KEY_F = 5,
			KEY_G = 6,
			KEY_H = 7,
			KEY_I = 8,
			KEY_J = 9,
			KEY_K = 10,
			KEY_L = 11,
			KEY_M = 12,
			KEY_N = 13,
			KEY_O = 14,
			KEY_P = 15,
			KEY_Q = 16,
			KEY_R = 17,
			KEY_S = 18,
			KEY_T = 19,
			KEY_U = 20,
			KEY_V = 21,
			KEY_W = 22,
			KEY_X = 23,
			KEY_Y = 24,
			KEY_Z = 25,
			KEY_0 = 26,
			KEY_1 = 27,
			KEY_2 = 28,
			KEY_3 = 29,
			KEY_4 = 30,
			KEY_5 = 31,
			KEY_6 = 32,
			KEY_7 = 33,
			KEY_8 = 34,
			KEY_9 = 35,
			LEFT_ARROW 	= 1 << 10,
			RIGHT_ARROW = 1 << 11,
			UP_ARROW 	= 1 << 12,
			DOWN_ARROW 	= 1 << 13,
			ALT 		= 1 << 14,
			CTRL 		= 1 << 15,
			SHIFT 		= 1 << 16
		};
	}

	class KeyboardEvent : public Event
	{
	public:
		KeyboardEvent() = default;
		KeyboardEvent(const EventType &type, Keyboard::key combination, Keyboard::key key, bool isPressed);
		~KeyboardEvent() = default;
		struct combination;

		const EventType		&getType() const noexcept;
		const EventCategory	&getCategory() const noexcept;
		const Keyboard::key	&getKey() const noexcept;
		const combination 	&getKeyCombination() const noexcept;
		bool 				isKeyPressed() const noexcept;
		bool 				isKeyReleased() const noexcept;

	public:
		Keyboard::key			_key;
		struct combination {
			combination() = default;
			combination(bool alt, bool ctrl, bool shift, bool caps_lock) :
			alt(alt), ctrl(ctrl), shift(shift), caps_lock(caps_lock) {}
			bool alt 		{false};
			bool ctrl		{false};
			bool shift		{false};
			bool caps_lock	{false};
		} _combination;
	};
}

#endif //ENGINE_KEYBOARDEVENT_HPP
