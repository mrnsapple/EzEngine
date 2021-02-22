/**
 * @file "Core/Exception/Private/manager_not_found.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "manager_not_found.hpp"
#include <string>

namespace ez
{
	manager_not_found::manager_not_found(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("No Manager Found: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	manager_not_found::manager_not_found(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("No Manager Found: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *manager_not_found::what() const noexcept
	{
		return _text.c_str();
	}
}