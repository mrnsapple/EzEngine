//
// Created by rcab on 23/10/2020.
//

#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP

#include "../../../config.hpp"
#include "XWindow.hpp"
#include "WNDWindow.hpp"

namespace ez
{
	class Window
	{
		public:
			Window( const char* windowTitle = "EzEngine Window", const  WindowSize& windowSize = WS_DEFAULT);
			~Window();

			void	Open()	{ _windowHandle->Open();	};
			void	Close()	{ _windowHandle->Close();	};
			int		SetWindowSize(const WindowSizeFormat& format);
			int 	SetWindowSize(const int& width, const int& height);
			bool	WasResized() noexcept;
			void 	SendEvents() noexcept;
		#if	defined(EZ_OS_LINUX)
			operator XWindow*()		const { return (GetLib() == WSLib::XLIB		? (XWindow*)_windowHandle	: nullptr); };
		#elif defined(EZ_OS_WINDOWS)
			operator WNDWindow*()	const { return (GetLib() == WSLib::WNDWINDOW ? (WNDWindow*)_windowHandle	: nullptr); };
		#endif

			// TODO: events and threads + thread safety
			bool		IsOpen()		const noexcept	{ return _windowHandle->IsOpen(); };
			WSLib		GetLib()		const noexcept	{ return _windowHandle->GetLib(); };
			WindowSize	GetScreenSize()	const noexcept	{ return _windowHandle->GetScreenSize(); };
			WindowSize	GetWindowSize()	const noexcept	{ return _windowHandle->GetWindowSize(); };

		private:
			IWindow* const _windowHandle;
			const WindowSize _screenSize;

			IWindow* initHandle(const  WindowSize& winSize, const char* winTitle);
	};
}

#endif //ENGINE_WINDOW_HPP