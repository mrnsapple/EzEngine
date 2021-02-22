/**
 * @file "Audio/Public/Device/AudioInfo.hpp"
 *
 * @author Alexandre Chetafi
 */


#ifndef ENGINE_AUDIOINFO_HPP
#define ENGINE_AUDIOINFO_HPP

#include <string>

namespace ez::Audio::details
{
	enum class SampleFormat : unsigned long
	{
		Float32	= 0x00000001,
		Int32	= 0x00000002,
		Int24	= 0x00000004,
		Int16	= 0x00000008,
		Int8	= 0x00000010,
		UInt8	= 0x00000020
	};

	struct AudioInfo
	{
		std::string	name;
		int 		inputChannels;
		int 		outputChannels;
		int 		index;
	};
}

#endif //ENGINE_AUDIOINFO_HPP
