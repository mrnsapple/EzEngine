/**
 * @file "ResourceManager/Public/Resource/ResourceData.cpp"
 *
 * @author Alexandre Chetafi
 *
 * From code of Oriol
 */

#ifndef ENGINE_RESOURCEDATAHOLDER_HPP
#define ENGINE_RESOURCEDATAHOLDER_HPP

#include <cstddef>

namespace ez
{
	class ResourceData {
	public:
		ResourceData() noexcept;
		ResourceData(void *data, std::size_t size) noexcept;

		void 		updateData(void *data, std::size_t size) noexcept;
		void 		*getData() const noexcept;
		void 		setData(void *data) noexcept;
		std::size_t getSize() const noexcept;
		void 		setSize(std::size_t size) noexcept;
	private:
		void 		*_data;
		std::size_t _size;
	};
}

#endif //ENGINE_RESOURCEDATAHOLDER_HPP
