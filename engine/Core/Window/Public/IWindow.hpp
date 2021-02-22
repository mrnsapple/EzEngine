/*
** EPITECH PROJECT, 2020
** ezengine
** File description:
** IWindow
*/

#ifndef _WINDOW_INTERFACE_
#define _WINDOW_INTERFACE_

#include <thread>
#include <mutex>
#include <string>
#include <atomic>

#include "WindowSize.hpp"
#include "ThreadPool.hpp"

namespace ez
{
	enum WSLib
	{
		XLIB = 0,
		WNDWINDOW
	};

	struct MwmHints {
		unsigned long flags;
		unsigned long functions;
		unsigned long decorations;
		long input_mode;
		unsigned long status;
	};

	enum {
		MWM_HINTS_FUNCTIONS = (1L << 0),
		MWM_HINTS_DECORATIONS =  (1L << 1),

		MWM_FUNC_ALL = (1L << 0),
		MWM_FUNC_RESIZE = (1L << 1),
		MWM_FUNC_MOVE = (1L << 2),
		MWM_FUNC_MINIMIZE = (1L << 3),
		MWM_FUNC_MAXIMIZE = (1L << 4),
		MWM_FUNC_CLOSE = (1L << 5)
	};

	class IWindow
	{
		public:
			IWindow(const WSLib& winLib, const WindowSize& winSize, const char* winTitle);
			virtual ~IWindow() = 0;

			void	Open();
			void	Close();
			bool	WasResized() noexcept;
			int		SetWindowSize(const WindowSizeFormat& format = WindowSizeFormat::CUSTOM, const int& width = 0, const int& height = 0) noexcept;

			bool		IsOpen()			const noexcept	{ return _isOpen;		};
			WSLib		GetLib()			const noexcept	{ return _windowLib;	};
			WindowSize	GetWindowSize()		const noexcept	{ return _windowSize;	};
			std::string	GetWindowTitle()	const noexcept	{ return std::string(_windowTitle);	};

			virtual WindowSize	GetScreenSize()	const noexcept = 0;
			virtual void initWindow()	= 0;
			virtual void sendEvent() = 0;

		protected:
			std::atomic<bool>	_isOpen;
			const WSLib			_windowLib;
			WindowSize			_windowSize;
			std::atomic<bool>	_windowResized;
			const char*	const	_windowTitle;

			virtual void deinitWindow()	= 0;
			virtual void openWindow()	= 0;
			virtual void closeWindow()	= 0;
			virtual void pollEvents()	= 0;
			virtual int	 setWindowSize(const WindowSize& size)	= 0;

		private:

			WindowSize _createWindowSize(const WindowSizeFormat& format = WindowSizeFormat::CUSTOM, const int& width = 0, const int& height = 0);

			static void pollEventsEntry(IWindow* win)
			{
				win->pollEvents();
			}
	};
}

#endif