/*
** EPITECH PROJECT, 2020
** ezEngine
** File description:
** main
*/
#include <gtest/gtest.h>

#include "VisualInstance.hpp"

#include <iostream>
#include "log.hpp"
#include "Window.hpp"
#include "WNDWindow.hpp"
using namespace ez;

#include "Init.hpp"
TEST(VT_MAIN, VT_MAINTEST)
{
    Init::create();
    Init::getPtr()->getSceneManager().CreateScene(true);
    EXPECT_EQ(1, 1);
    LOG(EZ_TRACE, "ENTER VTWNDWIN");
    //WNDWindow assss;
    LOG(EZ_TRACE, "ENTER VT MAIN");
    //Window a;
    LOG(EZ_TRACE, "After create window");

    bool appRunning = true;
    LOG(EZ_TRACE, "Before create visual instance");

    LOG(EZ_TRACE, "After create visual instance");
    //Init::getPtr()->getApp().Run();
}

// tests

//std::cin >> c;
//            if (c == 'x')
//                appRunning = false;

//            else if (c == 'a')
//                visual.SetWindowSize(800, 600);
//            else if (c == 'z')
//                visual.SetWindowSize(666, 101);
//            else if (c == 'e')
//                visual.SetWindowSize(10, 10);
//            else if (c == 'r')
//                visual.SetWindowSize(WindowSizeFormat::MINIMIZED);
//            else if (c == 't')
//                visual.SetWindowSize(WindowSizeFormat::MAXIMIZED);
//            else if (c == 'y')
//                visual.SetWindowSize(WindowSizeFormat::BORDERLESS);
//            else if (c == 'u')
//                visual.SetWindowSize(WindowSizeFormat::FULLSCREEN);

//            else if (c == 'q')
//                visual.SetWindowSize(-200, -100);
//            else if (c == 's')
//                visual.SetWindowSize(0, 10);
//            else if (c == 'd')
//                visual.SetWindowSize(69420, -100);
//            else if (c == 'f')
//                visual.SetWindowSize(200, 123456789);
//            else if (c == 'g')
//                visual.SetWindowSize(20000, 30000);

// - tests