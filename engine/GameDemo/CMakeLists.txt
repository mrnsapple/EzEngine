cmake_minimum_required(VERSION 3.13.4)
project(game)

include(../Core/core.cmake)

# Download and unpack googletest at configure time
link_directories( "D:/Program Files/VulkanSDK/1.2.148.1/Lib/")
include_directories( "D:/Program Files/VulkanSDK/1.2.148.1/Include/")

set (FILESPATH "./../Source/MemoryManagement/")

add_executable(game 
                Private/main.cpp
                Private/PlayerComponent.cpp
                Private/RotaryComponent.cpp
)
target_compile_definitions(game PUBLIC engine)

target_link_libraries(game PUBLIC  ${CORE_RENDERER_LIBDEP})
target_link_libraries(game PRIVATE engine ${CORE_RENDERER_LIBDEP})
