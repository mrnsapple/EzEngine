/**
 * @file "ResourceManager/Public/Resource/AudioOutputResource.cpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_AUDIORESOURCE__HPP
#define ENGINE_AUDIORESOURCE__HPP

#include "Resource.hpp"
#include "FileIO.hpp"
#include <memory>
#include "../../../config.hpp"

struct PaStreamCallbackTimeInfo;

namespace ez
{
	class ResourceData;
	namespace Audio
	{
	class AudioStreamOutput;
	}

	class AudioOutputResource : public Resource
	{
	public:
		static Resource *  Create(const ResourceData &data) { return new AudioOutputResource(data); }
		static Resource *  CreateFromPath(const std::string &path) { return new AudioOutputResource(path); }

		enum class State: uint8_t {PLAY, PAUSE, STOP};
		AudioOutputResource(const ResourceData &data);
		AudioOutputResource(const std::string &path);
		~AudioOutputResource();

		bool UpdateResourceData(const ResourceData &) override;
		bool OnEvent(Event &e) override;

		void		play(State s);

		/**
		 * Set the volume.
		 * @param volume The value must be between -1 and 1
		 */

		void		setVolume(float volume) noexcept;

		/**
		 * Get the actual Volume
		 * @return The volume
		 */

		float		getVolume() const noexcept;

		/**
		 * Try to know with some small uncertainty if the stream is still active
		 * @return true if playing, false otherwise
		 */

		bool		isPlaying() const noexcept;

		/**
		 * Try to know with some small uncertainty if the stream is in pause or stopped
		 * @return true if pause or stopped, false otherwise
		 */

		bool		isStopped() const noexcept;

	private:
		static int callback(const void *inputBuffer, void *outputBuffer,
							unsigned long framesPerBuffer,
							const PaStreamCallbackTimeInfo* timeInfo,
							unsigned long statusFlags,
							void *userData );

	private:
		float								_volume;
		FileIO::memory_file					*_id;
		std::unique_ptr<Audio::AudioStreamOutput>	_stream;
		struct parameters {
			parameters(void *a, void *b) : infos(a), handle(b) {}
			void 	*infos;
			void	*handle;
		}									_parameters;
	};
}

#endif //ENGINE_AUDIORESOURCE__HPP
