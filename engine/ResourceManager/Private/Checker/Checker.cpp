/**
 * @file "ResourceManager/Private/Checker/Checker.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "../../Public/Checker/Checker.hpp"
#include "../../Public/Resource/AudioOutputResource.hpp"
#include <fstream>
#include "../../Public/Resource/OBJResource.hpp"

#include <sndfile.h>

namespace ez
{
	bool Checker::checkWavFile(const std::string &filePath) noexcept
	{
		LOG_DETAIL(EZ_TRACE, "Checking .wav file in checkWavFile, file:" + filePath, __FILE__, __LINE__);
		SF_INFO _info;
		_info.format = 0;
		auto handle = sf_open(filePath.c_str(), SFM_READ, &_info);
		if (!handle) {
			std::cerr << sf_strerror(handle) << std::endl;
			return false;
		}
		sf_close(handle);
		return true;
	}

	bool Checker::checkOBJFile(const std::string &filePath) noexcept
	{
		LOG_DETAIL(EZ_TRACE, "Checking .obj file in checkWavFile, file:" + filePath + " ", __FILE__, __LINE__);
		try {
			auto tokenList = OBJResource::parseFile(filePath);
			if (tokenList.empty())
				return false;
		} catch (std::exception &) {
			return false;
		}
		return true;
	}

	bool Checker::checkIMGFile(const std::string &filePath) noexcept
	{
		return true;
	}

}