/**
 * @file "Core/Initializing/Public/Init.hpp"
 * Initialize all class for game engine
 * 
 * @author Remy Bouvard
 *
 * CHANGELOG :
 *
 * 17/10/2020 (Alexandre Chetafi)
 * Adding namespace ez
 * Fixing class name
 * Adding constructor and destructor default keyword
 * Preventing copy
 *
 * 04/11/2020 (Alexandre Chetafi
 * Shorting the name of the class
 * Adding static factory and getter
 * Adding getThreadPool method
 */

#ifndef ENGINE_INITIALIZE_HPP
#define ENGINE_INITIALIZE_HPP

#include <memory>
#include "ThreadPool.hpp"
#include "Event.hpp"
#include "SceneManager.hpp"
#include "ResourceManager.hpp"
#include "App.hpp"

#include <iostream>
#include "log.hpp"
#include "Window.hpp"
#include "WNDWindow.hpp"
#include "VisualInstance.hpp"
#include "UniqueIDManager.hpp"
#include "Initialization/Initialization.hpp"

namespace ez
{
	class ThreadPool;
   	class ResourceManager;
	class SceneManager;
	class App;
	class VisualInstance;
	class UniqueIDManager;
class Init
{
public:
	//Init() {};
	//~Init() = default;

	//Init(Init &) = delete;
	Init	&operator=(Init &) = delete;


	static std::shared_ptr<Init>	create();
	static void						destroy();
	static std::shared_ptr<Init>	getPtr();

	
	ThreadPool	&getThreadPool() noexcept;
	ResourceManager	&getResourceManager() noexcept;
	SceneManager	&getSceneManager() noexcept;
	App	&getApp() noexcept;
	VisualInstance &getVisual() noexcept;
	std::shared_ptr<UniqueIDManager>	 &getuniqueIDManager() noexcept;
	ez::Audio::Initialization &getAudioInit() noexcept;

private:
	ThreadPool	_pool;
	SceneManager  _sceneManager;
	App _app;
	ResourceManager _resourceManager;
	VisualInstance _visual;
	ez::Audio::Initialization audioinit;
	std::shared_ptr<UniqueIDManager> _uniqueIDManager=UniqueIDManager::create();

};
}

#endif //ENGINE_INITIALIZE_HPP