/**
 * @file "Core/Exception/Private/ouf_of_bound.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "../Public/out_of_bound.hpp"
#include <string>

namespace ez
{
	ouf_of_bound::ouf_of_bound(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Out of Bound: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	ouf_of_bound::ouf_of_bound(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Out of Bound: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *ouf_of_bound::what() const noexcept
	{
		return _text.c_str();
	}
}