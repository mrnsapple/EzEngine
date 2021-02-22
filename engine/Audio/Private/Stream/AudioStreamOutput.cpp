/**
 * @file "Audio/Private/Stream/AudioStreamOutput.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "Stream/AudioStreamOutput.hpp"
#include <portaudio.h>
#include <iostream>
namespace ez::Audio
{
	AudioStreamOutput::AudioStreamOutput(func_ptr fptr, double sampleRate, unsigned long framePerBuffer, void *arg) :
			_device(AudioDevice::Device::OutputDefault), _stream(nullptr), _fptr(fptr)
	{
		this->open(sampleRate, framePerBuffer, arg);
	}

	AudioStreamOutput::AudioStreamOutput(func_ptr fptr, double sampleRate, unsigned long framePerBuffer,
									   details::SampleFormat sampleFormat, void *arg) :
			_device(AudioDevice::Device::OutputDefault), _stream(nullptr), _fptr(fptr)
	{
		this->open(sampleRate, framePerBuffer, arg, sampleFormat);
	}

	AudioStreamOutput::~AudioStreamOutput()
	{
		if (!this->isStopped())
			this->abort();
	}

	bool AudioStreamOutput::isActive() noexcept
	{
		return Pa_IsStreamActive(this->_stream);
	}

	bool AudioStreamOutput::isStopped() noexcept
	{
		return Pa_IsStreamStopped(this->_stream);
	}

	void AudioStreamOutput::play()
	{
		auto error = Pa_StartStream(this->_stream);
		this->displayError(error);
	}

	void AudioStreamOutput::stop()
	{
		auto error = Pa_StopStream(this->_stream);
		this->displayError(error);
	}

	void AudioStreamOutput::open(double sampleRate, unsigned long framePerBuffer, void *arg) //TODO
	{
		auto error = Pa_OpenStream(&this->_stream, NULL, this->_device.getParameter(),
								   sampleRate, framePerBuffer, paNoFlag, _fptr, arg);
		this->displayError(error);
	}

	void AudioStreamOutput::open(double sampleRate, unsigned long framePerBuffer, void *arg,
								details::SampleFormat sampleFormat)
	{
		this->_device.getParameter()->sampleFormat = (unsigned long) sampleFormat;
		auto error = Pa_OpenStream(&this->_stream, NULL, this->_device.getParameter(),
								   sampleRate, framePerBuffer, paNoFlag, _fptr, arg);
		this->displayError(error);
	}

	void AudioStreamOutput::close()
	{
		auto error = Pa_CloseStream(this->_stream);
	}

	void AudioStreamOutput::abort()
	{
		auto error = Pa_AbortStream(this->_stream);
		this->displayError(error);
	}
}
