/**
 * @file "Audio/Public/Initialization/Initialization.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "Initialization/Initialization.hpp"
#include <portaudio.h>

namespace ez::Audio
{
	Initialization::Initialization() : _can_be_destroyed(true)
	{
		auto error = Pa_Initialize();
	}

	Initialization::~Initialization() noexcept
	{
		if (_can_be_destroyed)
			Pa_Terminate();
	}

	Initialization::Initialization(Initialization && other) noexcept : _can_be_destroyed(true)
	{
		other._can_be_destroyed = false;
	}

	Initialization & Initialization::operator=(Initialization &&other) noexcept
	{
		other._can_be_destroyed = false;
		return *this;
	}
}