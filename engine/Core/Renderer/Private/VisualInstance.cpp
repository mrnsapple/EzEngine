/*
** EPITECH PROJECT, 2020
** WindowSystem
** File description:
** VisualInstance
*/

#include "VisualInstance.hpp"
#include "Init.hpp"
using namespace ez;

VisualInstance::VisualInstance()
{
}

VisualInstance::~VisualInstance()
{
}

void VisualInstance::OpenWindow(std::vector<Mesh*> meshContainer)
{
	WS_handle.Open();
	VK_handle.Init(WS_handle, meshContainer);
}

void VisualInstance::CloseWindow(std::vector<Mesh*> meshContainer)
{
	VK_handle.Deinit(meshContainer);
	WS_handle.Close();
}

int VisualInstance::SetWindowSize(const int& width, const int& height)
{
	return WS_handle.SetWindowSize(width, height);
}

int VisualInstance::SetWindowSize(const WindowSizeFormat& format)
{
	return WS_handle.SetWindowSize(format);
}

void VisualInstance::DrawFrame(std::vector<Mesh*> meshContainer)
{
	if (WS_handle.WasResized())
		VK_handle.RecreateSwapChain(WS_handle, meshContainer);

	VK_handle.DrawFrame(WS_handle, meshContainer);
}

void VisualInstance::SendEvents()
{
	WS_handle.SendEvents();
}