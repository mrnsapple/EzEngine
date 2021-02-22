/*
** EPITECH PROJECT, 2020
** ezengine
** File description:
** XWindow
*/

#ifndef _XLIB_WINDOW_CLASS_
#define _XLIB_WINDOW_CLASS_

#include "../../../config.hpp"

#if defined(EZ_OS_LINUX)

#include "KeyBoardEvent.hpp"
#include "MouseEvent.hpp"
#include "IWindow.hpp"

#include <string>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace ez
{
	class XWindow : public IWindow
	{
		public:
			XWindow(const WindowSize& windowSize, const char* windowTitle);
			~XWindow();

			Display* 	GetDisplay		()	const noexcept { return _display;	}
			Window 	 	GetWindow		()	const noexcept { return _window;	}
			VisualID 	GetVisualID		()	const noexcept { return _visual->visualid; }
			// VisualID 	GetVisualID		()	const noexcept { return (_visual != nullptr ? _visual->visualid : 0); }
			WindowSize	GetScreenSize	()	const noexcept override;

		private:
			Display* _display;
			Window _window;
			int _screen;
			XSetWindowAttributes _attrs;
			unsigned long _attrs_mask;
			Visual* _visual;
			void sendEvent() override;
			inline void sendEvent(ez::Event &e);

			void initWindow		()							override;
			void deinitWindow	()							override;
			void openWindow		()							override;
			void closeWindow	()							override;
			bool createMouseMoveEvent(MouseEvent &event);
			bool createMouseEvent(bool isPressed, MouseEvent &event, XEvent _event);
			bool createKeyEvent(bool isPressed, KeyboardEvent &event, XEvent _event);
			void pollEvents		()							override;
			int	setWindowSize	(const WindowSize& size)	override;

			// Helper functions for windowResize
			int setBorderless(bool yesOrnNo);
			int setFullscreen();
			int minimize(bool yerOrNo);

			// int sendPropertyEvent(Atom action, Atom firstProp, Atom secondProp = 0);
			bool _isMinimized = false;
	};
}
#	endif
#endif