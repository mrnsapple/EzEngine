/**
 * @file "Core/Exception/Private/base.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "../Public/base.hpp"

namespace ez
{
	base_exception::base_exception(const std::string &str, const char *file, int line) : _string(str), _file(file), _line(line)
	{}

	base_exception::base_exception(const char *str, const char *file, int line) : _string(str), _file(file), _line(line)
	{}
}