/**
 * @file "ThreadPool_is_not_available.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_THREADPOOL_IS_NOT_AVAILABLE_HPP
#define ENGINE_THREADPOOL_IS_NOT_AVAILABLE_HPP

#include "base.hpp"

namespace ez
{
	class ThreadPool_is_not_available : public base_exception
	{
	public:
		ThreadPool_is_not_available(const std::string &str, char const *file, int line);
		ThreadPool_is_not_available(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};
}

#endif //ENGINE_THREADPOOL_IS_NOT_AVAILABLE_HPP
