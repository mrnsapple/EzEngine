/**
 * @file "Core/Exception/Private/renderer_exceptions.hpp"
 *
 * @author Remi Cabande
 */

#ifndef RENDERER_EXCEPTION_HPP
#define RENDERER_EXCEPTION_HPP

#include "base.hpp"

namespace ez
{
	// XLIB
	class xlib_failure : public base_exception
	{
	public:
		xlib_failure(const std::string &str, char const *file, int line);
		xlib_failure(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};


	// WNDCLASS
	class wnd_failure : public base_exception
	{
	public:
		wnd_failure(const std::string &str, char const *file, int line);
		wnd_failure(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};

	// VULKAN
	class vulkan_failure : public base_exception
	{
	public:
		vulkan_failure(const std::string &str, char const *file, int line);
		vulkan_failure(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};

	class vulkan_unavailable_validation_layers : public base_exception
	{
	public:
		vulkan_unavailable_validation_layers(const std::string &str, char const *file, int line);
		vulkan_unavailable_validation_layers(char const *str, char const *file, int line);
		const char *what() const noexcept final;
	private:
		std::string	_text;
	};
}

#endif //RENDERER_EXCEPTION_HPP
