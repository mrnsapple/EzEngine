/**
 * @file "Core/Exception/Private/ouf_of_bound.hpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_OUT_OF_BOUND_HPP
#define ENGINE_OUT_OF_BOUND_HPP

#include "base.hpp"

namespace ez
{
	class ouf_of_bound : public base_exception
	{
	public:
		ouf_of_bound(const std::string &str, char const *file, int line);
		ouf_of_bound(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};
}

#endif //ENGINE_OUT_OF_BOUND_HPP
