/*
** EPITECH PROJECT, 2020
** ezengine
** File description:
** WNDWindow
*/

#include "WNDWindow.hpp"

#if defined(EZ_OS_WINDOWS)

#include "renderer_exceptions.hpp"
#include "KeyBoardEvent.hpp"
#include "MouseEvent.hpp"
#include <windowsx.h>
#include "Init.hpp"
#include "log.hpp"
#include <strsafe.h>

namespace ez
{
	namespace
	{
		using namespace Keyboard;
		const int64_t invalid = -1;
		const int64_t array_translation_wparam_kevent[] =
				{
				/*   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F	*/
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0000
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0010
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0020
					KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, -1, -1, -1, -1, -1, -1, // 0x0030
					-1, KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, // 0x0040
					KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, -1, -1, -1, -1, -1, // 0x0050
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0060
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0070
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  // 0x0080
				};

		inline bool createKeyEvent(bool isPressed, const WPARAM &wParam, const LPARAM &lParam, KeyboardEvent &event)
		{
    		LOG(EZ_INFO, "Creating key events ");
			if (wParam > 0x8F || array_translation_wparam_kevent[wParam] == invalid) {
				LOG(EZ_ERROR, "Error creating key events ");
				return false;
			}
			event._key = (key) array_translation_wparam_kevent[wParam];
			event._type = isPressed ? EventType::KeyPressed : EventType::KeyReleased;
			event._combination.shift = GetKeyState(VK_SHIFT) >= 0; //TODO need verification in win7 and greater
			event._combination.ctrl = GetKeyState(VK_CONTROL) >= 0;
			event._combination.alt = GetKeyState(VK_MENU) >= 0;
			event._combination.caps_lock = GetKeyState(VK_CAPITAL) >= 0;
			return true;
		}

		using namespace Mouse;
		const int32_t array_translation_wparam_mevent[] =
				{
				/*   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F	*/
		/* 0x00 */	M_MOVE, M_LEFT, M_RIGHT, -1, SHIFT, -1, -1, -1, CTRL, -1, -1, -1, -1, -1, -1, -1,
		/* 0x10 */	M_CENTER, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
				};

		inline bool createMouseEvent(bool isPressed, const WPARAM &wParam, const LPARAM &lParam, MouseEvent &event)
		{
			event._key = (Mouse::key) array_translation_wparam_mevent[wParam];
			event._type = isPressed ? EventType::MouseButtonPressed : EventType::MouseButtonReleased;
			event._combination.ctrl = wParam & MK_CONTROL;
			event._combination.shift = wParam & MK_SHIFT;
			event._mouseX = GET_X_LPARAM(lParam);
			event._mouseY = GET_Y_LPARAM(lParam);
			return true;
		}

		inline bool createMouseMoveEvent(const LPARAM &lParam, MouseEvent &event) {
			event._key = M_MOVE;
			event._type = EventType::MouseMoved;
			event._mouseX = GET_X_LPARAM(lParam);
			event._mouseY = GET_Y_LPARAM(lParam);
			return true;
		}

		inline void sendEvent(Event &e) {
			SceneManager &scenemanager = Init::getPtr()->getSceneManager();
			scenemanager.getActiveScene()->OnEvent(e);
			return;
		}

	
// Callback function linked to the internal window system class
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			KeyboardEvent	k_event;
			MouseEvent		m_event;

			switch (msg) {
				case WM_KEYDOWN:
					if (createKeyEvent(false, wParam, lParam, k_event)) { sendEvent(k_event);}; break;
				case WM_KEYUP:
					if (createKeyEvent(true, wParam, lParam, k_event)) { sendEvent(k_event);}; break;
				case WM_MOUSEMOVE:
					if (createMouseMoveEvent(lParam, m_event)) {sendEvent(m_event);}; break;
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
					if (createMouseEvent(true, wParam, lParam, m_event)) {sendEvent(m_event);}; break;
				case WM_LBUTTONUP:
				case WM_MBUTTONUP:
				case WM_RBUTTONUP:
					if (createMouseEvent(false, wParam, lParam, m_event)) {sendEvent(m_event);}; break;
				case WM_CLOSE:
				case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
				case WM_PAINT:/* {
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hwnd, &ps);

					// All painting occurs here, between BeginPaint and EndPaint.

					FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

					EndPaint(hwnd, &ps);
					return 0;
				}*/
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			return 0;
		}
	}

	WNDWindow::WNDWindow(const WindowSize &winSize, const char *winTitle) :
			IWindow(WSLib::WNDWINDOW, winSize, winTitle)
	{
		initWindow();
	}

	WNDWindow::~WNDWindow()
	{
		closeWindow();
		deinitWindow();
	}

	void WNDWindow::displayError(const char * lpszFunction)
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );
		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
		ExitProcess(dw);
	}

	void WNDWindow::sendToPump(const windowEventPump &event)
	{
		const std::lock_guard<std::mutex>	_lock(this->_mutex);
		buffer.push(event);
	}

	WNDWindow::windowEventPump WNDWindow::receiveFromPump()
	{
		windowEventPump event {windowEventPump::EventPump::NONE};
		const std::lock_guard<std::mutex>	_lock(this->_mutex);
		if (!buffer.empty()) {
			event = buffer.front();
			buffer.pop();
		}
		return event;
	}

	void	WNDWindow::executePump()
	{
		auto event = this->receiveFromPump();
		switch (event._event) {
			case windowEventPump::EventPump::NONE: break;
			case windowEventPump::EventPump::DESTROY: DestroyWindow(hwnd); break;
			case windowEventPump::EventPump::OPEN: ShowWindow(hwnd, SW_SHOWNORMAL); UpdateWindow(hwnd); break;
			case windowEventPump::EventPump::CLOSE: ShowWindow(hwnd, SW_HIDE); UpdateWindow(hwnd); break;
		}
	}

	void WNDWindow::sendEvent() 
	{
		MSG Msg = {};

		this->executePump();
		if (GetMessage(&Msg, NULL, 0, 0) <= 0)
			return;
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	void WNDWindow::initWindow()
	{
		LOG(EZ_TRACE, "IN INIT WINDOW ");

		auto hinstance = GetModuleHandle(NULL);

		/* Window class arguments */
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		// Window Procedure pointer, aka where every action is sent when something happens
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		// Icone of the application on ALT+TAB, no application icon yet so deal with it
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		// Cursor of the window
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		// Menu ressource to use for the window
		wc.lpszMenuName = NULL;
		// Name of the internal window class
		wc.lpszClassName = "EZWindowClass";
		// Icone of the application at the top left corner and in task bar
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wc)) {
			displayError("wc.lpszClassName");
			throw wnd_failure("to register Window Class EX structure", __FILE__, __LINE__);
		}
		this->hwnd = CreateWindow(wc.lpszClassName, "EZ Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
							_windowSize.width, _windowSize.height, NULL, NULL, hinstance, NULL);
		if (hwnd == NULL) {
			displayError("hwnd");
			throw wnd_failure("to get the handle of the window", __FILE__, __LINE__);
		}
		this->openWindow();


	}

	void WNDWindow::deinitWindow()
	{
		const windowEventPump	event {windowEventPump::EventPump::DESTROY};
		this->sendToPump(event);
		//DestroyWindow(hwnd);
	}

	void WNDWindow::openWindow()
	{
		const windowEventPump	event {windowEventPump::EventPump::OPEN};
		this->sendToPump(event);
	}

	void WNDWindow::closeWindow()
	{
		const windowEventPump	event {windowEventPump::EventPump::CLOSE};
		this->sendToPump(event);
	}

	void WNDWindow::pollEvents()
	{
		//Do nothing due to thread windows' sensibility
	}

	int WNDWindow::setWindowSize(const WindowSize &size)
	{
		if (size.format == SW_MAXIMIZE)
			ShowWindow(hwnd, SW_MAXIMIZE);
		else if (size.format == MINIMIZED)
			ShowWindow(hwnd, SW_MINIMIZE);
		else
			MoveWindow(hwnd, 200, 100, size.width, size.height, TRUE);
		_windowSize = size;
		UpdateWindow(hwnd);
		return 0;
	}

	WindowSize WNDWindow::GetScreenSize() const noexcept
	{
		return _windowSize;
	}

/*
// Unit Test Methods
bool WNDWindow::TestWCisNULL() const noexcept
{
	return (wc == NULL);
}*/

	bool WNDWindow::TestHWNDisNULL() const noexcept
	{
		return (hwnd == NULL);
	}

	HWND WNDWindow::GetHWND() const noexcept
	{
		return this->hwnd;
	}

}
#endif