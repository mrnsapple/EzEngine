/**
 * @file "Core/Exception/Private/renderer_exceptions.cpp"
 *
 * @author Remi Cabande
 */

#include "renderer_exceptions.hpp"

namespace ez
{

	// ############
	// #   XLIB   #
	// ############

	xlib_failure::xlib_failure(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("XLib: Failed " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	xlib_failure::xlib_failure(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("XLib: Failed " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *xlib_failure::what() const noexcept
	{
		return _text.c_str();
	}



	// ################
	// #   WNDCLASS   #
	// ################

	wnd_failure::wnd_failure(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("WNDClass: Failed " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	wnd_failure::wnd_failure(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("WNDClass: Failed " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *wnd_failure::what() const noexcept
	{
		return _text.c_str();
	}



	// ##############
	// #   VULKAN   #
	// ##############

	vulkan_failure::vulkan_failure(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Vulkan: Failed " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	vulkan_failure::vulkan_failure(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Vulkan: Failed " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *vulkan_failure::what() const noexcept
	{
		return _text.c_str();
	}



	vulkan_unavailable_validation_layers::vulkan_unavailable_validation_layers(const char *str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Vulkan: Validation layers requested but not available: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	vulkan_unavailable_validation_layers::vulkan_unavailable_validation_layers(const std::string &str, const char *file, int line) : base_exception(str, file, line)
	{
		_text = std::string("Vulkan: Validation layers requested but not available: " + this->_string + ", file : " + this->_file + " @ line :" + std::to_string(this->_line));
	}

	const char *vulkan_unavailable_validation_layers::what() const noexcept
	{
		return _text.c_str();
	}
}