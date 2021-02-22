#include <gtest/gtest.h>
#include "SceneManager.hpp"
#include "SceneComponent.hpp"
#include <memory>
#include "Scene.hpp"
#include "AudioComponent.hpp"
#include "Collision.hpp"
#include "Init.hpp"
//#define EZ_LOG 4
#include "log.hpp"
#include "VisualInstance.hpp"

using namespace ez;
using namespace details;
TEST(SMALLGAME, START) {
    std::shared_ptr<Init>  i( Init::create());
    std::cout << "START SMALL GAME" << std::endl;
    LOG(EZ_TRACE, "HELLO CHETAFI");
    LOG_DETAIL(EZ_TRACE, "BYEBYE CHETAFI", __FILE__, __LINE__);
    LOG_DETAIL(EZ_ERROR, "BYEBYE CHETAFI", __FILE__, __LINE__);
    VisualInstance visual;
	ez::log::write_terminal("ceci est un test unitaire");
    std::cout << "START SMALL GAME" << std::endl;

    EXPECT_EQ(12,12);

    //i->getApp().Run();
    
    //i.getThreadPool().getThreadPool()->dispatch(_func_run);
    std::cout << "AFTER LOOP" << std::endl;
    EXPECT_EQ(1,1);
}

// TEST(SMALLGAME, START) {
//     std::shared_ptr<Init>  i( Init::create());
//     std::cout << "START SMALL GAME" << std::endl;
//     LOG(EZ_TRACE, "HELLO CHETAFI");
//     LOG_DETAIL(EZ_TRACE, "BYEBYE CHETAFI", __FILE__, __LINE__);
//     LOG_DETAIL(EZ_ERROR, "BYEBYE CHETAFI", __FILE__, __LINE__);

// 	ez::log::write_terminal("ceci est un test unitaire");
//     std::cout << "START SMALL GAME" << std::endl;

//     EXPECT_EQ(2,1);

//     std::cout << "START SMALL GAME" << std::endl;
//     auto _func_run = []() {
//         std::cout << "IN FUNC" << std::endl;
//         i->getApp().Run();
//         EXPECT_EQ(1,1);
//     };
//     ThreadPool &a = i->getThreadPool();
//     //i.getThreadPool().getThreadPool()->dispatch(_func_run);
//     std::cout << "AFTER LOOP" << std::endl;
//     EXPECT_EQ(1,1);
// }