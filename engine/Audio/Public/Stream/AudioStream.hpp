/**
 * @file "Audio/Public/Stream/AudioStream.hpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_STREAM_HPP
#define ENGINE_STREAM_HPP

struct PaStreamCallbackTimeInfo;

namespace ez::Audio
{
	class AudioStream
	{
	public:
		using func_ptr = int (*)(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo *, unsigned long, void *);
		AudioStream() = default;
		virtual ~AudioStream() = default;
		AudioStream(AudioStream &) = delete;
		AudioStream	&operator=(const AudioStream &) = delete;

		virtual bool	isActive() noexcept = 0;
		virtual bool 	isStopped() noexcept = 0;
		virtual void 	play() = 0;
		virtual void 	stop() = 0;

	protected:
		virtual void 	open(double sampleRate, unsigned long framePerBuffer, void *arg) = 0;
		virtual void 	close() = 0;
		virtual void 	abort() = 0;
		void 			displayError(int);
	};
}

#endif //ENGINE_STREAM_HPP
