/**
 * @file "Audio/Private/Device/AudioDevice.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "Device/AudioDevice.hpp"
#include "out_of_range.hpp"
#include <portaudio.h>

namespace ez::Audio
{
	AudioDevice::AudioDevice(Device d) : _parameters(new PaStreamParameters)
	{
		switch (d) {
			case Device::InputDefault: {
				_index = Pa_GetDefaultInputDevice();
				if (_index == paNoDevice)
					throw out_of_range("No index found due to no input device found", __FILE__, __LINE__);
				this->registerDevice();
				break;
			}
			case Device::OutputDefault : {
				_index = Pa_GetDefaultOutputDevice();
				if (_index == paNoDevice)
					throw out_of_range("No index found due to no output device found", __FILE__, __LINE__);
				this->registerDevice();
				break;
			}
		}
		this->createStreamParameters();
	}

	AudioDevice::AudioDevice(int index) : _index(index), _parameters(new PaStreamParameters)
	{
		if (index >= Pa_GetDeviceCount() || index < 0)
			throw out_of_range("", __FILE__, __LINE__);
		this->registerDevice();
		this->createStreamParameters();
	}

	std::vector<details::AudioInfo> AudioDevice::getAllAudioDevice() noexcept
	{
		std::vector<details::AudioInfo>	v(Pa_GetDeviceCount());
		for (auto i = 0; i < v.size(); i++) {
			auto _tmp = Pa_GetDeviceInfo(i);
			v[i].name = _tmp->name;
			v[i].inputChannels = _tmp->maxInputChannels;
			v[i].outputChannels = _tmp->maxOutputChannels;
			v[i].index = i;
		}
		return v;
	}

	int AudioDevice::defaultIndexInput()
	{
		return Pa_GetDefaultInputDevice() != paNoDevice ? Pa_GetDefaultInputDevice() : -1;
	}

	int AudioDevice::defaultIndexOutput()
	{
		return Pa_GetDefaultOutputDevice() != paNoDevice ? Pa_GetDefaultOutputDevice() : -1;
	}

	const std::string & AudioDevice::getName() const noexcept
	{
		return this->_name;
	}

	int AudioDevice::getCanalsNumber() const noexcept
	{
		return this->_canals;
	}

	int AudioDevice::getIndex() const noexcept
	{
		return this->_index;
	}

	PaStreamParameters * AudioDevice::getParameter() const noexcept
	{
		return this->_parameters.get();
	}

	void AudioDevice::registerDevice()
	{
		auto _tmp = Pa_GetDeviceInfo(this->_index);
		this->_name = _tmp->name;
		this->_canals = _tmp->maxOutputChannels ? _tmp->maxOutputChannels : _tmp->maxInputChannels;
		this->_canals = this->_canals > 2 ? 2 : this->_canals; //limiting channels
	}

	void AudioDevice::createStreamParameters()
	{
		auto _tmp = Pa_GetDeviceInfo(this->_index);
		_parameters->device = _index;
		_parameters->channelCount = _canals;
		_parameters->hostApiSpecificStreamInfo = NULL;
		_parameters->suggestedLatency = _tmp->maxOutputChannels ? _tmp->defaultLowOutputLatency : _tmp->defaultLowInputLatency;
		_parameters->sampleFormat = paInt16; // default
	}
}