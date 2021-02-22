/*
** EPITECH PROJECT, 2020
** engine
** File description:
** WindowTest
*/

#include <gtest/gtest.h>

#include "Window.hpp"

TEST(WindowTest, OpenWindow)
{
    try
    {
        // ez::Window* window = new ez::Window("Window Test");
        ez::Window window;

        window.Open();
        EXPECT_TRUE(window.IsOpen());
        window.Close();
        EXPECT_FALSE(window.IsOpen());
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}

TEST(WindowTest, TestDefaultValues)
{
//     ez::Window* window = new ez::Window("Window Test");
    ez::Window window;

#if defined(EZ_OS_LINUX)
    EXPECT_EQ(window.GetLib(), ez::WSLib::XLIB);
    EXPECT_NE(((ez::XWindow*)window)->GetDisplay(), nullptr);
    EXPECT_NE(((ez::XWindow*)window)->GetVisualID(), 0);
    ez::WindowSize winSize = ((ez::XWindow*)window)->GetWindowSize();

#elif defined(EZ_OS_WINDOWS)
    EXPECT_EQ(window.GetLib(), ez::WSLib::WNDWINDOW);
//    EXPECT_FALSE(((ez::WNDWindow*)window)->TestWCisNULL());
    EXPECT_FALSE(((ez::WNDWindow*)window)->TestHWNDisNULL());
    ez::WindowSize winSize = (((ez::WNDWindow*)window)->GetWindowSize());

#endif
    EXPECT_EQ(winSize.format, ez::WindowSizeFormat::CUSTOM);
    EXPECT_EQ(winSize.width, 720);
    EXPECT_EQ(winSize.height, 480);

}