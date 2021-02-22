/*
** EPITECH PROJECT, 2020
** ezengine
** File description:
** XWindow
*/

#include "XWindow.hpp"

#if defined(EZ_OS_LINUX)

#include "log.hpp"
#include "renderer_exceptions.hpp"

namespace ez
{
	XWindow::XWindow(const WindowSize& winSize, const char* winTitle)	:
		IWindow(WSLib::XLIB, winSize, winTitle)
	{
		initWindow();
	}

	XWindow::~XWindow()
	{
		Close();
		deinitWindow();
	}

	WindowSize XWindow::GetScreenSize() const noexcept
	{
		WindowSize result;
		Screen* screen = XScreenOfDisplay(_display, _screen);

		result.format = WindowSizeFormat::CUSTOM;
		result.width = XWidthOfScreen(screen);
		result.height = XHeightOfScreen(screen);

		return result;
	}

	void XWindow::initWindow()
	{
		// ThreadSafePrint("XWindow::initWindow\n");
		// ENABLE MULTI-THREADING SUPPORT
		if (XInitThreads() == 0)
			throw ez::xlib_failure("MultiThreadSupport initialisation (XInitThreads())", __FILE__, __LINE__);

		// INIT DISPLAY
		_display = XOpenDisplay(nullptr);
		if (_display == nullptr)
			throw ez::xlib_failure("to open display (XOpenDisplay())", __FILE__, __LINE__);

		// INIT SCREEN
		_screen = DefaultScreen(_display);

		// INIT COLORS ?

		// INIT WINDOW ATTRIBUTES
		_attrs.event_mask
			= KeyPressMask
			| KeyReleaseMask
			| ButtonPressMask
			| ButtonReleaseMask
			| EnterWindowMask
			| LeaveWindowMask
			| PointerMotionMask
			| KeymapStateMask
			| VisibilityChangeMask
			| StructureNotifyMask;
			// | SubstructureNotifyMask;
			// | ResizeRedirectMask;
			// | SubstructureRedirectMask;
			// | 

		_attrs_mask = CWEventMask;

		// INIT WINDOW
		_window = XCreateWindow(
			_display,						// X_server
			DefaultRootWindow(_display),	// parent =_window
			0, 0,							// x, y pos (relative to container window ?)
			_windowSize.width, _windowSize.height,		// width, height
			0,								// border_width
			CopyFromParent,					// depth
			InputOutput,					// class
			CopyFromParent,					// Visual* visual ?
			_attrs_mask, &_attrs			// attribute_mask, attributes
		);

		// GET VISUAL ID FOR VK
		XWindowAttributes tmpattrs;
		if (XGetWindowAttributes(_display, _window, &tmpattrs))
			_visual = tmpattrs.visual;
		else
			throw ez::xlib_failure("to match visual info (XGetWindowAttributes())", __FILE__, __LINE__);
		
		// ThreadSafePrint("XWindow::initWindow - Done\n");
	}

	void XWindow::deinitWindow()
	{
		// ThreadSafePrint("XWindow::deinitWindow\n");

		if (_display != nullptr)
		{
			XSetCloseDownMode(_display, DestroyAll);
			XCloseDisplay(_display);
			_display = nullptr;
		}
	}

	void XWindow::openWindow()
	{
		// ThreadSafePrint("XWindow::openWindow\n");

		XMapWindow(_display, _window);
		XStoreName(_display, _window, _windowTitle);

		for(;;) {
			XEvent e;
			XNextEvent(_display, &e);
			if (e.type == MapNotify)
				break;
		}
	}

	void XWindow::closeWindow()
	{
		// ThreadSafePrint("XWindow::closeWindow\n");

		if (_window)
		{
			XUnmapWindow(_display, _window);
			XDestroyWindow(_display, _window);
		}
	}

	using namespace ez::Mouse;

	bool XWindow::createMouseMoveEvent(MouseEvent &event)
	{
		event._key = M_MOVE;
		event._type = EventType::MouseMoved;

		Window root_window; 
		int root_x, root_y; 
		unsigned int mask;  
		XQueryPointer(_display, DefaultRootWindow(_display), &root_window, &root_window, &root_x, &root_y, &root_x, &root_y, &mask);

		event._mouseX = root_x;
		event._mouseY = root_y;
		return true;
	}

	bool XWindow::createMouseEvent(bool isPressed, MouseEvent &event, XEvent _event)
	{
		//event._key = (Mouse::key) array_translation_wparam_mevent[wParam];
		event._type = isPressed ? EventType::MouseButtonPressed : EventType::MouseButtonReleased;
		event._combination.ctrl = _event.xkey.state & ControlMask;
		event._combination.shift = _event.xkey.state & ShiftMask;
		Window root_window; 
		int root_x, root_y; 
		unsigned int mask;  
		XQueryPointer(_display, DefaultRootWindow(_display), &root_window, &root_window, &root_x, &root_y, &root_x, &root_y, &mask);

		event._mouseX = root_x;
		event._mouseY = root_y;
		return true;
	}

	bool XWindow::createKeyEvent(bool isPressed, KeyboardEvent &event, XEvent _event)
	{
		//if (wParam > 0x8F || array_translation_wparam_kevent[wParam] == invalid)
		//	return false;
		using namespace Keyboard;

		event._key = (ez::Keyboard::key) _event.xkey.keycode;
		event._type = isPressed ? EventType::KeyPressed : EventType::KeyReleased;
		event._combination.shift = _event.xkey.state & ShiftMask;
		event._combination.ctrl = _event.xkey.state & ControlMask;
		event._combination.alt = _event.xkey.state & Mod1Mask;
		event._combination.caps_lock = _event.xkey.state & LockMask;
		return true;
	}

	inline void XWindow::sendEvent(ez::Event &e)
	{
		// SceneManager &scenemanager = Init::getPtr()->getSceneManager();
		// scenemanager.getActiveScene()->OnEvent(e);
		return;
	}

	void XWindow::sendEvent()
	{
		return;
	}

	void XWindow::pollEvents()
	{
		// ThreadSafePrint("XWindow::pollEvents\n");
		std::string str;

		while (this->IsOpen())
		{
			XEvent 			event;
			KeyboardEvent 	k_event;
			MouseEvent 		m_event;

			XNextEvent(_display , &event);

			switch (event.type)
			{
				case KeyPress:
					if (createKeyEvent(true, k_event, event)) {sendEvent(k_event);};
					str = std::string("KeyPress(" + std::to_string(event.type) + ") : " + std::to_string(event.xkey.keycode) + '\n');
					break;

				case KeyRelease:
					if (createKeyEvent(false, k_event, event)) {sendEvent(k_event);};
					str = std::string("KeyRelease(" + std::to_string(event.type) + ") : " + std::to_string(event.xkey.keycode) + '\n');
					break;

				case ButtonPress:
					if (createMouseEvent(true, m_event, event)) {sendEvent(m_event);};
					str = std::string("ButtonPress(" + std::to_string(event.type) + ") : " + std::to_string(event.xbutton.button) + '\n');
					break;

				case ButtonRelease:
					if (createMouseEvent(false, m_event, event)) {sendEvent(m_event);};
					str = std::string("ButtonRelease(" + std::to_string(event.type) + ") : " + std::to_string(event.xbutton.button) + '\n');
					break;

				case MotionNotify:
					if (createMouseMoveEvent(m_event)) {sendEvent(m_event);};
					str = std::string("PointerMoved(" + std::to_string(event.type) + ") : " + std::to_string(event.xmotion.x) + ", " + std::to_string(event.xmotion.y) + '\n');
					break;
				
				case ConfigureNotify:
					str = std::string("WindowResize(" + std::to_string(event.type) + ") : w=" + std::to_string(event.xconfigure.width) + ", h=" + std::to_string(event.xconfigure.height) + "\n");
					if (event.xconfigure.width != _windowSize.width || event.xconfigure.height != _windowSize.height)
						SetWindowSize(WindowSizeFormat::CUSTOM, event.xconfigure.width, event.xconfigure.height);
					break;

				default:
					break;
			}
			// if (str.size() > 1)
			// {
			// 	std::cout << str;
			// 	str = std::string("0");
			// }
		}
	}

	int XWindow::setWindowSize(const WindowSize& size)
	{
		LOG(EZ_TRACE, "Resizing XWindow");

		int result = 0;

		switch (size.format)
		{
			case WindowSizeFormat::CUSTOM:
				result = minimize(false);
				result = (result == 0 ? 0 : XResizeWindow(_display, _window, size.width, size.height));
				result = (result == 0 ? 0 : setBorderless(true));
				break;

			case WindowSizeFormat::MINIMIZED:
				result = minimize(true);
				// result = (result == 0 ? 0 : XMoveResizeWindow(_display, _window, 0, 0, size.width, size.height));
				break;

			case WindowSizeFormat::MAXIMIZED:
				result = minimize(false);
				result = (result == 0 ? 0 : XMoveResizeWindow(_display, _window, 0, 0, size.width, size.height));
				result = (result == 0 ? 0 : setBorderless(false));
				break;

			case WindowSizeFormat::BORDERLESS:
				result = minimize(false);
				result = (result == 0 ? 0 : XMoveResizeWindow(_display, _window, 0, 0, size.width, size.height));
				result = (result == 0 ? 0 : setBorderless(true));
				break;

			case WindowSizeFormat::FULLSCREEN:
				result = minimize(false);
				result = (result == 0 ? 0 : XMoveResizeWindow(_display, _window, 0, 0, size.width, size.height));
				result = (result == 0 ? 0 : setBorderless(true));
				// result = (result == 0 ? 0 : setFullscreen());
				break;

			default:
				return 0;
		}

		return result;
	}

	// Helper Functions
	int XWindow::setBorderless(bool yesOrNo)
	{
		Atom mwmHintsProperty = XInternAtom(_display, "_MOTIF_WM_HINTS", 0);
		if (mwmHintsProperty == 0)
			return 0;

		struct MwmHints hints;
		hints.flags = MWM_HINTS_DECORATIONS;
		hints.decorations = (int)yesOrNo;
		int result = XChangeProperty(
			_display, _window,
			mwmHintsProperty, mwmHintsProperty,
			32,	PropModeReplace, (unsigned char *)&hints, 5
		);

		// int result = XSetWindowBorderWidth(_display, _window, 0);
		// printf("XChangeProperty = %i\n", result);
		return 1;
	}

	int XWindow::setFullscreen()
	{
		int result = 0;

		result = XMapRaised(_display, _window);
		// result = (result == 0 ? 0 : XGrabPointer(_display, _window, True, 0, GrabModeAsync, GrabModeAsync, _window, 0L, CurrentTime));
		// result = (result == 0 ? 0 : XGrabKeyboard(_display, _window, False, GrabModeAsync, GrabModeAsync, CurrentTime));

		return result;
	}

	int XWindow::minimize(bool yesOrNo)
	{
		int result = 1;

		if (yesOrNo && !_isMinimized)
		{
			LOG(EZ_TRACE, "Minimizing XWindow");
			result = XIconifyWindow(_display, _window, _screen);
			_isMinimized = true;
		}
		else if (!yesOrNo && _isMinimized)
		{
			LOG(EZ_TRACE, "Un-minimizing XWindow");
			result = XDeleteProperty(_display, _window, MWM_FUNC_MINIMIZE);
			result = (result == 0 ? 0 : XMapRaised(_display, _window));
			_isMinimized = (result == 0 ? true : false);
		}

		return result;
	}

	// #include <cstring>

	// int XWindow::sendPropertyEvent(Atom action, Atom firstProp, Atom secondProp)
	// {
	// 	XEvent xev;
	// 	Atom wm_state = XInternAtom(_display, "_NET_WM_STATE", False);

	// 	memset(&xev, 0, sizeof(xev));
	// 	xev.type = ClientMessage;
	// 	xev.xclient.window = _window;
	// 	xev.xclient.message_type = wm_state;
	// 	xev.xclient.format = 32;
	// 	xev.xclient.data.l[0] = action;
	// 	xev.xclient.data.l[1] = firstProp;
	// 	xev.xclient.data.l[2] = secondProp;

	// 	int result = XSendEvent(_display, DefaultRootWindow(_display), False, SubstructureNotifyMask, &xev);
	// 	printf("send event = %i\n", result);

	// 	return result;
	// }
}

#endif