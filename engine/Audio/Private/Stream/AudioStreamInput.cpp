/**
 * @file "Audio/Private/Stream/AudioStreamInput.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "Stream/AudioStreamInput.hpp"
#include "out_of_bound.hpp"
#include <portaudio.h>

namespace ez::Audio
{
	AudioStreamInput::AudioStreamInput(func_ptr fptr, double sampleRate, unsigned long framePerBuffer, void *arg) :
	_device(AudioDevice::Device::OutputDefault), _stream(nullptr), _fptr(fptr)
	{
		this->open(sampleRate, framePerBuffer, arg);
	}

	AudioStreamInput::AudioStreamInput(func_ptr fptr, double sampleRate, unsigned long framePerBuffer,
									   details::SampleFormat sampleFormat, void *arg) :
			_device(AudioDevice::Device::OutputDefault), _stream(nullptr), _fptr(fptr)
	{
		this->open(sampleRate, framePerBuffer, arg, sampleFormat);
	}

	AudioStreamInput::~AudioStreamInput()
	{
		if (!this->isStopped())
			this->abort();
	}

	bool AudioStreamInput::isActive() noexcept
	{
		return Pa_IsStreamActive(this->_stream);
	}

	bool AudioStreamInput::isStopped() noexcept
	{
		return Pa_IsStreamStopped(this->_stream);
	}

	void AudioStreamInput::play()
	{
		auto error = Pa_StartStream(this->_stream);
		this->displayError(error);
	}

	void AudioStreamInput::stop()
	{
		auto error = Pa_StopStream(this->_stream);
		this->displayError(error);
	}

	void AudioStreamInput::open(double sampleRate, unsigned long framePerBuffer, void *arg)
	{
		auto error = Pa_OpenStream(&this->_stream, this->_device.getParameter(), NULL,
							 sampleRate, framePerBuffer, paNoFlag, _fptr, arg);
		this->displayError(error);
	}

	void AudioStreamInput::open(double sampleRate, unsigned long framePerBuffer, void *arg,
								details::SampleFormat sampleFormat)
	{
		this->_device.getParameter()->sampleFormat = (unsigned long) sampleFormat;
		auto error = Pa_OpenStream(&this->_stream, this->_device.getParameter(), NULL,
								   sampleRate, framePerBuffer, paNoFlag, _fptr, arg);
		this->displayError(error);
	}

	void AudioStreamInput::close()
	{
		auto error = Pa_CloseStream(this->_stream);
		this->displayError(error);
	}

	void AudioStreamInput::abort()
	{
		auto error = Pa_AbortStream(this->_stream);
		this->displayError(error);
	}
}