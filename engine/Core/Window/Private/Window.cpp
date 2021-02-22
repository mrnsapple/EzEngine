/*
** EPITECH PROJECT, 2020
** engine
** File description:
** Window
*/

#include <algorithm>
#include "Window.hpp"
namespace ez {
Window::Window(const  char* windowTitle, const WindowSize& windowSize):	
	_windowHandle(initHandle(windowSize, windowTitle)),
	_screenSize(GetScreenSize())
{
	// std::cout << "AFTER CALL INITHANDLE IN WINDOW" << std::endl <<  std::endl;
}

void 	Window::SendEvents() noexcept
{
	if(!_windowHandle)
		return;

	_windowHandle->sendEvent();
}


Window::~Window()
{
	Close();
	delete _windowHandle;
}

int Window::SetWindowSize(const WindowSizeFormat& format)
{
	return _windowHandle->SetWindowSize(format);
}

int Window::SetWindowSize(const int& width, const int& height)
{
	return _windowHandle->SetWindowSize(WindowSizeFormat::CUSTOM, width, height);
}

bool Window::WasResized() noexcept
{
	return _windowHandle->WasResized();
}

IWindow* Window::initHandle(const  WindowSize& winSize, const char* winTitle)
{
#if defined(EZ_OS_LINUX)
	return new XWindow(winSize, winTitle);
#elif defined(EZ_OS_WINDOWS)
	return new WNDWindow(winSize, winTitle);
#endif
}

}