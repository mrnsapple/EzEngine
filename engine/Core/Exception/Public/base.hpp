/**
 * @file "Core/Exception/Private/base.hpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_BASE_EXCEPTION_HPP
#define ENGINE_BASE_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace ez
{
	class base_exception : public std::exception
	{
	public:

		/**
		 * This constructor take a message, the name of the file and lines of the appealing exception
		 * @param str A message
		 * @param file The name of the source file
		 * @param line Line of the code
		 */

		base_exception(const std::string &str, char const *file, int line);


		/**
		 * This constructor take a message, the name of the file and lines of the appealing exception
		 * @param str A message
		 * @param file The name of the source file
		 * @param line Line of the code
		 */

		base_exception(char const *str, char const *file, int line);
	protected:
		const int 			_line;
		const std::string 	_string;
		const std::string 	_file;
	};
}

#endif // ENGINE_BASE_EXCEPTION_HPP