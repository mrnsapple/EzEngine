/*
** EPITECH PROJECT, 2020
** WindowSystem
** File description:
** VisualInstance
*/

#include "../../../config.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#ifndef _EZ_ENGINE_VISUAL_INSTANCE_
#define _EZ_ENGINE_VISUAL_INSTANCE_



namespace ez
{
	class Window;
	class VisualInstance
	{
		public:
			VisualInstance();
			~VisualInstance();
			void OpenWindow(std::vector<Mesh*> meshContainer);
			void CloseWindow(std::vector<Mesh*> meshContainer);
			int SetWindowSize(const int& width, const int& height);
			int SetWindowSize(const WindowSizeFormat& size);
			void DrawFrame(std::vector<Mesh*> meshContainer);
			void SendEvents();

		private:
			Window WS_handle;
			Renderer VK_handle;
	};
}

#endif