/*
** EPITECH PROJECT, 2020
** engine
** File description:
** Settings
*/

#pragma once

#include <stdint.h>

namespace ez
{
	static bool enableValidationLayers = true;
	static bool enableAnisotropy = false;
	static bool enableTextureSampling = false;

	static uint8_t msaaLevel = 1;
	const int MAX_FRAMES_IN_FLIGHT = 5;

#if defined(EZ_OS_LINUX)
    const std::string SHADER_VERT_PATH	= "../Core/Renderer/shaders/vert.spv";
    const std::string SHADER_FRAG_PATH	= "../Core/Renderer/shaders/frag.spv";
    const std::string MODEL_PATH		= "../../assets/models/viking_room.obj";
    const std::string VIKING_ROOM_TEXTURE_PATH		= "../../assets/textures/viking_room.png";
    const std::string STATUE_TEXTURE_PATH		= "../../assets/textures/statue.jpg";
    const std::string ALBIN_TEXTURE_PATH		= "../../assets/textures/albinTex.png";
#elif defined(EZ_OS_WINDOWS)
	const std::string SHADER_VERT_PATH	= "D:/Epitech/GameEngine/ezengine/engine/Core/Renderer/shaders/vert.spv";
	const std::string SHADER_FRAG_PATH	= "D:/Epitech/GameEngine/ezengine/engine/Core/Renderer/shaders/frag.spv";
	const std::string MODEL_PATH		= "D:/Epitech/GameEngine/ezengine/engine/../assets/models/viking_room.obj";
	const std::string VIKING_ROOM_TEXTURE_PATH		= "D:/Epitech/GameEngine/ezengine/engine/../assets/textures/viking_room.png";
	const std::string STATUE_TEXTURE_PATH		= "D:/Epitech/GameEngine/ezengine/engine/../assets/textures/statue.jpg";
	const std::string ALBIN_TEXTURE_PATH		= "D:/Epitech/GameEngine/ezengine/engine/../assets/textures/albinTex.png";

#endif

}
