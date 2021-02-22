/**
 * @file "Audio/Public/Stream/AudioStreamOutput.hpp"
 *
 * @author Alexandre Chetafi
 */


#ifndef ENGINE_AUDIOSTREAMOUTPUT_HPP
#define ENGINE_AUDIOSTREAMOUTPUT_HPP

#include "AudioStream.hpp"
#include "Device/AudioDevice.hpp"

namespace ez::Audio
{
	class AudioStreamOutput : public AudioStream
	{
	public:
		AudioStreamOutput(func_ptr fptr, double sampleRate, unsigned long framePerBuffer, void *userData = NULL);
		AudioStreamOutput(func_ptr fptr, double sampleRate, unsigned long framePerBuffer, details::SampleFormat sampleFormat, void *userData = NULL);
		~AudioStreamOutput() override;

		bool	isActive() noexcept final;
		bool 	isStopped() noexcept final;
		void 	play() final;
		void 	stop() final;

	private:
		void 	open(double sampleRate, unsigned long framePerBuffer, void *arg) final;
		void 	open(double sampleRate, unsigned long framePerBuffer, void *arg, details::SampleFormat sampleFormat);
		void 	close() final;
		void 	abort() final;

	private:
		AudioDevice	_device;
		void		*_stream;
		func_ptr	_fptr;
	};
}

#endif //ENGINE_AUDIOSTREAMOUTPUT_HPP
