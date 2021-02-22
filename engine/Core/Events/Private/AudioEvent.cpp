/**
 * @file "Core/Events/Private/ImageResource.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "AudioEvent.hpp"

namespace ez
{
	AudioEvent::AudioEvent(EventType type)
	{
		this->_type = type;
	}

	EventType AudioEvent::getType() const noexcept
	{
		return this->_type;
	}
}