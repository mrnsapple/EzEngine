/**
 * @file "Audio/Public/Device/AudioDevice.hpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_AUDIODEVICE_HPP
#define ENGINE_AUDIODEVICE_HPP

#include <string>
#include <vector>
#include <memory>
#include "AudioInfo.hpp"

struct PaStreamParameters;

namespace ez::Audio
{
	class AudioDevice
	{
	public:
		enum class Device {OutputDefault, InputDefault};
		explicit AudioDevice(Device d);
		explicit AudioDevice(int index);

		static std::vector<details::AudioInfo>	getAllAudioDevice() noexcept;
		static int 	defaultIndexInput();
		static int	defaultIndexOutput();

		const std::string 	&getName() const noexcept;
		int 				getCanalsNumber() const noexcept;
		int 				getIndex() const noexcept;
		PaStreamParameters	*getParameter() const noexcept;

	private:
		void	registerDevice();
		void	createStreamParameters();
	private:
		int 		_index;
		int 		_canals;
		std::string	_name;
		std::unique_ptr<PaStreamParameters>	_parameters;
	};
}

#endif //ENGINE_AUDIODEVICE_HPP
