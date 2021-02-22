/*
** EPITECH PROJECT, 2020
** ezengine
** File description:
** WindowSize
*/

#ifndef _WINDOW_SIZES_
#define _WINDOW_SIZES_

#include <stdint.h>
namespace ez
{
	enum WindowSizeFormat
	{
		CUSTOM = -1,	// any size

		MINIMIZED = 0,	// == Minimized / Invisible
		MAXIMIZED,		// == Windowed Fullscreen
		BORDERLESS,		// == Borderless Windowed Fullscreen
		FULLSCREEN,		// == Fullscreen
	};

	struct WindowSize
	{
		WindowSizeFormat format;
		uint32_t width;
		uint32_t height;
	};

	// Default 480p window
	static WindowSize WS_DEFAULT
	{
		WS_DEFAULT.format = WindowSizeFormat::BORDERLESS,
		WS_DEFAULT.width = 720,
		WS_DEFAULT.height = 480
	};
}

#endif
