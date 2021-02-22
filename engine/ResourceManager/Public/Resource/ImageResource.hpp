/**
 * @file "ResourceManager/Public/Resource/ImageResource.cpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_IMAGERESOURCE_HPP
#define ENGINE_IMAGERESOURCE_HPP

#include "Resource.hpp"

namespace ez
{
	class ResourceData;

	class ImageResource : public Resource
	{
	public:
		static Resource *  Create(const ResourceData &data) { return new ImageResource(data); }
		static Resource *  CreateFromPath(const std::string &path) { return new ImageResource(path); }

		bool UpdateResourceData(const ResourceData &);
		bool OnEvent(Event &) override {return false;}

		ImageResource(const ResourceData &);
		ImageResource(const std::string  &);

		~ImageResource() = default;

		/**
		 * Get the width of the image
		 * @return a size
		 */

		std::size_t getWidth() const noexcept;

		/**
		 * Get the length of the image
		 * @return a size
		 */

		std::size_t getHeight() const noexcept;

		/**
		 * Get the number of channels for this image
		 * @return The number of channels
		 */

		std::size_t getChannel() const noexcept;


		/**
		 * Get the pixel table from the image
		 * @return A array of uint8_t conataining pixels data
		 */

		uint8_t	*getPixel() const noexcept;

	private:
		void 								*_handle;
		int _width, _height, _channel;
	};
}

#endif //ENGINE_IMAGERESOURCE_HPP
