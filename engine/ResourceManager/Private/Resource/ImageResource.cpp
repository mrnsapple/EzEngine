/**
 * @file "ResourceManager/Private/Resource/ImageResource.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "Resource/ImageResource.hpp"
#include "Resource/ResourceData.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>
#include "log.hpp"
#include <iostream>
namespace ez
{
	bool ImageResource::UpdateResourceData(const ResourceData &data)
	{
		this->_handle = stbi_load_from_memory((stbi_uc *) data.getData(), data.getSize(),
											  &this->_width, &this->_height, &this->_channel, STBI_rgb_alpha);
		if (!this->_handle)
			throw std::logic_error("Can not load the image into memory");
		return this->_handle;
	}

	ImageResource::ImageResource(const std::string  &path)
	{
		LOG_DETAIL(EZ_TRACE, "In ImageResource Constructor ", __FILE__, __LINE__);

		// std::cout << path << std::endl;
		this->_handle = stbi_load(path.c_str(), &this->_width, &this->_height, &this->_channel, STBI_rgb_alpha);
		if (!this->_handle)
			throw std::logic_error("Can not load the image into memory");
	}

	ImageResource::ImageResource(const ResourceData &data)
	{
		LOG_DETAIL(EZ_TRACE, "In ImageResource Constructor ", __FILE__, __LINE__);

		this->_handle = stbi_load_from_memory((stbi_uc *) data.getData(), data.getSize(),
										&this->_width, &this->_height, &this->_channel, STBI_rgb_alpha);
		if (!this->_handle)
			throw std::logic_error("Can not load the image into memory");
	}

	std::size_t ImageResource::getHeight() const noexcept
	{
		return this->_height;
	}

	std::size_t ImageResource::getWidth() const noexcept
	{
		return this->_width;
	}

	std::size_t ImageResource::getChannel() const noexcept
	{
		return this->_channel;
	}

	uint8_t *ImageResource::getPixel() const noexcept
	{
		return (uint8_t*) this->_handle;
	}
}