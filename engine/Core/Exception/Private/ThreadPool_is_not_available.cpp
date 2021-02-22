/**
 * @file "ThreadPool_is_not_availble.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "ThreadPool_is_not_available.hpp"

namespace ez
{
	ThreadPool_is_not_available::ThreadPool_is_not_available(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Thread Pool does not exist or is not available " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	ThreadPool_is_not_available::ThreadPool_is_not_available(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Thread Pool does not exist or is not available " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *ThreadPool_is_not_available::what() const noexcept
	{
		return _text.c_str();
	}
}