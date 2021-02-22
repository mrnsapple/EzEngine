/**
 * @file "Core/Initializing/Private/Init.cpp"
 * Initialize all class for game engine
 *
 * @author Alexandre Chetafi
 *
 */

#include "Init.hpp"

namespace ez
{
	namespace {
		std::shared_ptr<Init>	_ptr {nullptr};
	}

	std::shared_ptr<Init> Init::create()
	{
		if (!_ptr)
			_ptr = std::shared_ptr<Init>(new Init);
		return _ptr;
	}

	void Init::destroy()
	{
		if (_ptr) {
			_ptr.reset();
			_ptr = nullptr;
		}
	}

	std::shared_ptr<Init> Init::getPtr()
	{
		return _ptr;
	}

	ThreadPool &Init::getThreadPool() noexcept
	{
		return this->_pool;
	}

	ResourceManager &Init::getResourceManager() noexcept
	{
		return this->_resourceManager;
	}

	Audio::Initialization &Init::getAudioInit() noexcept
	{
		return this->audioinit;
	}

	SceneManager &Init::getSceneManager() noexcept
	{
		return this->_sceneManager;
	}
	
	std::shared_ptr<UniqueIDManager>	 &Init::getuniqueIDManager() noexcept
	{
		return this->_uniqueIDManager;

	}

	
	App &Init::getApp() noexcept
	{
		return this->_app;
	}

	VisualInstance &Init::getVisual() noexcept
	{
		return this->_visual;
	}


}
