/*
** EPITECH PROJECT, 2020
** engine
** File description:
** IWindow
*/

#include "IWindow.hpp"
#include "Init.hpp"
using namespace ez;

IWindow::IWindow(const WSLib& winLib, const WindowSize& winSize, const char* winTitle)	:
	_isOpen(false),
	_windowLib(winLib),
	_windowSize(winSize),
	_windowTitle(winTitle)
{
}

IWindow::~IWindow()
{
}

void IWindow::Open()
{
	if (_isOpen)
	{
		// printf("Window Already Open\n");
		return;
	}

	_isOpen = true;
	openWindow();
	Init::getPtr()->getThreadPool().getThreadPool()->post(pollEventsEntry, this);
}

void IWindow::Close()
{
	if (!_isOpen)
	{
		// printf("Window Already Closed\n");
		return;
	}

	_isOpen = false;
	closeWindow();
}

int IWindow::SetWindowSize(const WindowSizeFormat& format, const int& width, const int& height) noexcept
{
	if (format == _windowSize.format && width == _windowSize.width && height == _windowSize.height)
		return 1;

	WindowSize size = _createWindowSize(format, width, height);

	int result = setWindowSize(size);

	if (result == 1) {
		_windowSize = size;
		_windowResized = true;
	}

	return result;
}

bool IWindow::WasResized() noexcept
{
	if (_windowResized)
	{
		_windowResized = false;
		return true;
	}

	return false;
}

WindowSize IWindow::_createWindowSize(const WindowSizeFormat& format, const int& width, const int& height)
{
	WindowSize size;
	size.format = format;

	if (format == WindowSizeFormat::CUSTOM)
	{
		WindowSize screenSize = GetScreenSize();
		size.width	= std::max(0, std::min((int)screenSize.width,	width));
		size.height = std::max(0, std::min((int)screenSize.height,	height));

		if (width <= 0 || height <= 0)
			size.format = WindowSizeFormat::MINIMIZED;
		else if (width >= screenSize.width && height >= screenSize.height)
			size.format = WindowSizeFormat::MAXIMIZED;
	}
	else if (format == WindowSizeFormat::MINIMIZED)
	{
		size.width = 0;
		size.height = 0;
	}
	else // if (format >= WindowSizeFormat::MAXIMIZED)
	{
		WindowSize screenSize = GetScreenSize();
		size.width = screenSize.width;
		size.height = screenSize.height;
	}
	
	return size;
}