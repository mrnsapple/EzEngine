/**
 * @file "ResourceManager/Private/Resource/ResourceData.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "Resource/ResourceData.hpp"

namespace ez
{
	ResourceData::ResourceData() noexcept : _data(nullptr), _size(0)
	{}

	ResourceData::ResourceData(void *data, std::size_t size) noexcept : _data(data), _size(size)
	{}

	void ResourceData::updateData(void *data, std::size_t size) noexcept
	{
		this->_data = data;
		this->_size = size;
	}

	void * ResourceData::getData() const noexcept
	{
		return this->_data;
	}

	std::size_t ResourceData::getSize() const noexcept
	{
		return this->_size;
	}

	void ResourceData::setData(void *data) noexcept
	{
		this->_data = data;
	}

	void ResourceData::setSize(std::size_t size) noexcept
	{
		this->_size = size;
	}
}