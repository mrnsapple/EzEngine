/**
 * @file "Core/Exception/Private/manager_not_found.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "fileIOFailed.hpp"
#include <string>

namespace ez
{
	fileIOFailed::fileIOFailed(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("FileIO failed: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	fileIOFailed::fileIOFailed(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("FileIO failed: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *fileIOFailed::what() const noexcept
	{
		return _text.c_str();
	}
}