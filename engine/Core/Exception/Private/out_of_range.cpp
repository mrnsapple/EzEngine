/**
 * @file "Core/Exception/Private/out_of_range.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "../Public/out_of_range.hpp"
#include <string>

namespace ez
{
	out_of_range::out_of_range(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Out of Range: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	out_of_range::out_of_range(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Out of Range: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *out_of_range::what() const noexcept
	{
		return _text.c_str();
	}
}