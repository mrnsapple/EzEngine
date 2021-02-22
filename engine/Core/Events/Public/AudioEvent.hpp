/**
 * @file "Core/Events/Public/ImageResource.cpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_AUDIOEVENT_HPP
#define ENGINE_AUDIOEVENT_HPP

#include "Event.hpp"

namespace ez
{
	class AudioEvent : public Event
	{
	public:
		AudioEvent(EventType );

		EventType	getType() const noexcept;

	public:
		const EventCategory _category =  EventCategory::EventAudio;
	};
}

#endif //ENGINE_AUDIOEVENT_HPP
