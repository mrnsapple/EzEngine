/**
 * @file "Core/Exception/Private/out_of_range.hpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_OUT_OF_RANGE_HPP
#define ENGINE_OUT_OF_RANGE_HPP

#include "base.hpp"

namespace ez
{
	class out_of_range : public base_exception
	{
	public:
		out_of_range(const std::string &str, char const *file, int line);
		out_of_range(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};
}

#endif //ENGINE_OUT_OF_RANGE_HPP
