/**
 * @file "Audio/Private/Stream/AudioStream.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "Stream/AudioStream.hpp"
#include <portaudio.h>
#include <iostream>

namespace ez::Audio
{
	void AudioStream::displayError(int error)
	{
		if (error != paNoError) {
			std::cerr << Pa_GetErrorText(error) << std::endl;
		}
	}
}