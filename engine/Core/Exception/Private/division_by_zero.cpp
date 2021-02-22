/**
 * @file "division_by_zero.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "../Public/division_by_zero.hpp"

namespace ez
{
	division_by_zero::division_by_zero(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Division by Zero: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	division_by_zero::division_by_zero(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Division by Zero: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *division_by_zero::what() const noexcept
	{
		return _text.c_str();
	}
}