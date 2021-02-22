/**
 * @file "ResourceManager/Private/Resource/AudioOutputResource.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "Resource/ResourceData.hpp"
#include "Resource/AudioOutputResource.hpp"
#include "Stream/AudioStreamOutput.hpp"
#include <sndfile.h>
#include <portaudio.h>
#include <cstring>
#include "AudioEvent.hpp"
#include "log.hpp"
#ifndef FALSE
#define FALSE 0
#endif

namespace ez
{
	using namespace Audio;

	AudioOutputResource::AudioOutputResource(const ResourceData &data) : _parameters(new SF_INFO, nullptr),
		_id(FileIO::createFileFromMemory(data)), _volume(.5f)
	{
		LOG_DETAIL(EZ_TRACE, "In AudioOutputResource Constructor", __FILE__, __LINE__);

		std::memset(_parameters.infos, 0, sizeof(SF_INFO));
		_parameters.handle = sf_open_fd(_id->fd, SFM_READ, (SF_INFO *) _parameters.infos, FALSE);
		if (!_parameters.handle) {
			std::cerr << sf_strerror(NULL) << std::endl;
			FileIO::deleteFileFromMemory(&_id);
			throw std::logic_error("Can not open stream");
		}
		auto *infos = (SF_INFO *) _parameters.infos;

		_stream = std::make_unique<AudioStreamOutput>(callback, infos->samplerate, 512, details::SampleFormat::Int16, this);
	}

	AudioOutputResource::AudioOutputResource(const std::string &path):_parameters(new SF_INFO, nullptr), _volume(.5f), _id(nullptr)
	{
		LOG_DETAIL(EZ_TRACE, "In AudioOutputResource Constructor", __FILE__, __LINE__);
		std::cout << path << std::endl;
		std::ifstream file(path);
		_parameters.handle =  sf_open(path.c_str(), SFM_READ, (SF_INFO *) _parameters.infos);
		if (!_parameters.handle) {

			throw std::logic_error(sf_strerror(NULL));
		}
		auto *infos = (SF_INFO *) _parameters.infos;
		
		_stream = std::make_unique<AudioStreamOutput>(callback, infos->samplerate, 512, details::SampleFormat::Int16, this);

	}

	AudioOutputResource::~AudioOutputResource()
	{
		this->_stream->stop();
		sf_close((SNDFILE *) this->_parameters.handle);
		if (this->_id)
			FileIO::deleteFileFromMemory(&_id);

		delete (SF_INFO *) this->_parameters.infos;
	}

	bool AudioOutputResource::UpdateResourceData(const ResourceData &data)
	{
		return false;
	}

	bool AudioOutputResource::OnEvent(Event &e)
	{
		if (e._category == EventCategory::EventAudio) {
			switch (e._type) {
				case EventType::AudioPause: this->play(State::PAUSE); return true;
				case EventType::AudioPlay:	this->play(State::PLAY); return true;
				case EventType::AudioStop:	this->play(State::STOP); return true;
				default:
					return false;
			}
		}
		return false;
	}

	void AudioOutputResource::play(State s)
	{
		switch (s) {
			case State::PLAY:
				this->_stream->play(); break;
			case State::STOP: {
				this->_stream->stop();
				sf_seek((SNDFILE *) this->_parameters.handle, 0, SEEK_SET); break;
			}
			case State::PAUSE:
				this->_stream->stop(); break;
		}
	}

	void AudioOutputResource::setVolume(float volume) noexcept
	{
		this->_volume = volume;
	}

	float AudioOutputResource::getVolume() const noexcept
	{
		return this->_volume;
	}

	bool AudioOutputResource::isPlaying() const noexcept
	{
		return this->_stream->isActive();
	}

	bool AudioOutputResource::isStopped() const noexcept
	{
		return this->_stream->isStopped();
	}

	int AudioOutputResource::callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
									  const PaStreamCallbackTimeInfo *timeInfo, unsigned long statusFlags,
									  void *userData)
	{
		auto	_resource = (AudioOutputResource *) userData;
		auto	_file = (SNDFILE *) _resource->_parameters.handle;
		auto	_infos = (SF_INFO *) _resource->_parameters.infos;

		auto frame_read = sf_read_short(_file, (short *) outputBuffer, framesPerBuffer *  _infos->channels);
		if (frame_read < framesPerBuffer)
			return paComplete;
		return paContinue;
	}
}