#include <gtest/gtest.h>
#include "ResourceManager.hpp"
#include "ThreadPoolInternal.hpp"
    #include <stdlib.h> 

#include <filesystem>
#include "Init.hpp"
#include "ImageResource.hpp"
namespace fs = std::filesystem;
using namespace ez;

using namespace details;

TEST(RESOURCE_MANAGER, Test_ADD_image) {
    std::shared_ptr<Init> i = Init::create();
	ResourceManager &_manager = Init::getPtr()->getResourceManager();

    std::string path = fs::current_path().parent_path().parent_path().u8string();
    path += "/engine/__tests__/ResourceManager/scull.jpeg";

    std::pair<std::shared_ptr<UniqueID> , std::string> id = _manager.saveResource(path);
    EXPECT_NE(nullptr, std::get<0>(id).get() );
    std::string binarypath = std::get<1>(id);
    ImageResource *res = (ImageResource *)_manager.loadResource(binarypath);
    
    if (res) {
        std::cout << res->getWidth() << std::endl;
        std::cout << res->getHeight() << std::endl;
        std::cout << res->getChannel() << std::endl;
        EXPECT_EQ(1,1);
    }
    else
        EXPECT_EQ(1,2);
}



// TEST(RESOURCE_MANAGER, Test_wav) {
//     ResourceManager _manager;
//     std::string path = fs::current_path().parent_path().parent_path().parent_path().u8string();
//     path += "\\engine\\__tests__\\ResourceManager\\LRMonoPhase4.wav";
//     std::cout << "Current path in wav is " << path<< '\n';

//     std::pair<std::shared_ptr<UniqueID> , std::string> id = _manager.saveResource(path);
//      std::shared_ptr<UniqueID>uniqueid(std::get<0>(id));
//     std::cout << "unique id:" <<   std::endl;
//     std::cout << uniqueid << std::endl;
//     std::cout << uniqueid.get() << std::endl;

//     EXPECT_NE(nullptr, uniqueid.get() );
//     std::cout << "End Current path  in wav is " << path<< '\n';
//     std::string binarypath = std::get<1>(id);
//     Resource *res = _manager.loadResource(binarypath);
// }

// TEST(RESOURCE_MANAGER, Test_ADD_obj) {
//     ResourceManager _manager;
//     std::string path = fs::current_path().parent_path().parent_path().parent_path().u8string();
//     path += "\\engine\\__tests__\\ResourceManager\\wOLFHEAD1.obj";
//     std::cout << "Current path in obj is " << path<< '\n';

//     std::pair<std::shared_ptr<UniqueID> , std::string> id = _manager.saveResource(path);
//      std::shared_ptr<UniqueID>uniqueid(std::get<0>(id));
//     std::cout << "unique id:" <<   std::endl;
//     std::cout << uniqueid << std::endl;
//     std::cout << uniqueid.get() << std::endl;

//     EXPECT_NE(nullptr, uniqueid.get() );
//     std::cout << "End Current path  in obj is " << path<< '\n';
//     std::string binarypath = std::get<1>(id);
//     Resource *res = _manager.loadResource(binarypath);
// }


// TEST(RESOURCE_MANAGER, Test_ADD_THREAD) {
//     ResourceManager *_manager = new ResourceManager;
//     std::string path = fs::current_path().parent_path().parent_path().parent_path().u8string();
//     path += "\\engine\\__tests__\\ResourceManager\\wOLFHEAD1.obj";
//     std::cout << "Current path ina dd thread " << path<< '\n';
// 	auto _t = ThreadPoolInternal::create();
//     std::pair<std::shared_ptr<UniqueID> , std::string> id =  std::make_pair(std::shared_ptr<UniqueID>(),"");
// 	auto _func = [&id, _manager](std::string path) {
//       id =_manager->saveResource(path);
//         EXPECT_NE(nullptr, std::get<0>(id).get() );

// 	};
//     _t->dispatch(_func, path);
//     _t->dispatch(_func, path);
//     _t->dispatch(_func, path);
//     std::cout << "End save calls" << path<< '\n';
//     //EXPECT_EQ(NULL, std::get<0>(id).lock() );

//     std::thread _tt([&]{
// 		_t->run();
// 	});

//     if (_tt.joinable())
// 		_tt.join();

//     // /EXPECT_NE(nullptr, std::get<0>(id) );
//     std::string binarypath = std::get<1>(id);
//     std::shared_ptr<UniqueID> myid = std::get<0>(id);
//     std::cout << "End Current path is inadd thread" << binarypath<< '\n';
//     EXPECT_NE(nullptr, myid.get());
//     //Resource *res = _manager.loadResource(binarypath);
// }

