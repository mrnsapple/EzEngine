/**
 * @file "Core/Exception/Public/manager_not_found.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_MANAGER_NOT_FOUND_HPP
#define ENGINE_MANAGER_NOT_FOUND_HPP

#include "base.hpp"

namespace ez
{
	class manager_not_found : public base_exception
	{
	public:
		manager_not_found(const std::string &str, char const *file, int line);
		manager_not_found(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};
}

#endif //ENGINE_MANAGER_NOT_FOUND_HPP
