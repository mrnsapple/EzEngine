/** 
 * @author Oriol Manzano
 */

#include <gtest/gtest.h>
#include "SceneManager.hpp"
#include "SceneComponent.hpp"
#include <memory>
#include "Scene.hpp"
#include "AudioComponent.hpp"
#include "Collision.hpp"

#include "Stream/AudioStreamOutput.hpp"

#include <fstream>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>
#include "ResourceManager.hpp"
#include "ThreadPoolInternal.hpp"
#include <stdlib.h> 

#include <filesystem>
#include <iostream>
#include <portaudio.h>
#include <sndfile.h>

#include "Initialization/Initialization.hpp"
#include "AudioOutputResource.hpp"
using namespace ez;
using namespace Audio;

namespace fs = std::filesystem;
#include "Init.hpp"




TEST(SceneTest, SceneCompoludnentsChilds) {
	std::string  path = fs::current_path().parent_path().parent_path().parent_path().u8string();
	path += "\\engine\\__tests__\\Audio\\LRMonoPhase4.wav";
    std::shared_ptr<Init> i = Init::create();

   
	ResourceManager &_manager = i->getResourceManager();
	std::pair<std::shared_ptr<UniqueID> , std::string> id = _manager.saveResource(path);
    EXPECT_NE(nullptr, std::get<0>(id).get() );
    std::string binarypath = std::get<1>(id);
    AudioOutputResource *audioOutputResource = (AudioOutputResource *)_manager.loadResource(binarypath);
    auto f = [](AudioOutputResource *audioOutputResource) {
    try {
		//AudioOutputResource audioOutputResource(path);
		audioOutputResource->play(AudioOutputResource::State::PLAY);
		do {
			std::this_thread::sleep_for(std::chrono::seconds(5));
		} while (audioOutputResource->isPlaying());
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
    };
    std::thread thread_object(f,audioOutputResource); 

    while(true){}
}
