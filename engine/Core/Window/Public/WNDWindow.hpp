/*
** EPITECH PROJECT, 2020
** ezengine
** File description:
** XWindow
*/

#ifndef _WND_WINDOW_CLASS_
#define _WND_WINDOW_CLASS_

#include "../../../config.hpp"

#if	defined(EZ_OS_WINDOWS)

#include <string>
#include "IWindow.hpp"
#include <mutex>
#include <queue>
namespace ez
{
	class WNDWindow : public IWindow {
	public:
		WNDWindow(const WindowSize &size = WS_DEFAULT, const char *winTitle = "EZ Engine");
		~WNDWindow();

		// Unit Test Methods
		void sendEvent() override;

		bool TestWCisNULL() const noexcept;
		bool TestHWNDisNULL() const noexcept;
		HWND	GetHWND() const noexcept;

	private:
		WNDCLASSEX wc;
		HWND hwnd;

		struct windowEventPump {
			enum class EventPump {
				NONE,
				DESTROY,
				CLOSE,
				OPEN,

			}		_event;
		};

		void 		displayError(const char *lpszFunction);
		void 		sendToPump(const windowEventPump &);
		void		executePump();
		windowEventPump 	receiveFromPump();

		std::queue<windowEventPump>	buffer;
		std::mutex		_mutex;

		void 	initWindow() override;
		void 	deinitWindow() override;
		void 	openWindow() override;
		void 	closeWindow() override;
		void 	pollEvents() override;
		int 	setWindowSize(const WindowSize &size) override;

		WindowSize GetScreenSize() const noexcept override;
	};
}
#endif // _WIN32
#endif // _WND_WINDOW_CLASS_