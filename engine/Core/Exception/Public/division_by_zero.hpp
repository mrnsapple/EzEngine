/**
 * @file "division_by_zero.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_DIVISIOIN_BY_ZERO_HPP
#define ENGINE_DIVISIOIN_BY_ZERO_HPP

#include "base.hpp"

namespace ez
{
	class division_by_zero : public base_exception
	{
	public:
		division_by_zero(const std::string &str, char const *file, int line);
		division_by_zero(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};
}

#endif //ENGINE_DIVISIOIN_BY_ZERO_HPP
