cmake_minimum_required(VERSION 3.13.4)
project(engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

configure_file(Core/Renderer/Public/Settings.in ${CMAKE_SOURCE_DIR}/Core/Renderer/Public/Settings.hpp)

include(Physics/physics.cmake)
include(App/app.cmake)
include(Core/core.cmake)
include(GameObjects/gameobjects.cmake)
include(Audio/audio.cmake)

include(ResourceManager/resourcemanager.cmake)

include_directories(${CORE_INCLUDE} ${APP_INCLUDE} ${GAMEOBJECTS_INCLUDE} ${AUDIO_INCLUDE} ${RESOURCE_MANAGER_INCLUDES} ${PHYSICS_INCLUDES})
#include_directories(${CORE_INCLUDE} ${APP_INCLUDE} ${GAMEOBJECTS_INCLUDE} ${RESOURCE_MANAGER_INCLUDES} ${PHYSICS_INCLUDES})



# add_library(engine STATIC ${CORE} ${GAMEOBJECTS} ${PHYSICS}  ${APP} ${AUDIO} ${RESOURCE_MANAGER} ${RESOURCE_MANAGER})
add_library(engine STATIC ${CORE} ${GAMEOBJECTS} ${PHYSICS} ${APP} ${AUDIO} ${RESOURCE_MANAGER} ${RESOURCE_MANAGER} ${PHYSICS})
target_link_libraries(engine ${LIBS_CURRENT} ${CONAN_LIBS})
# target_include_directories(engine PUBLIC ${CORE_INCLUDES} ${PHYSICS_INCLUDES} ${AUDIO_INCLUDE} ${RESOURCE_MANAGER_INCLUDES})
target_include_directories(engine PUBLIC ${CORE_INCLUDES} ${PHYSICS_INCLUDES} ${AUDIO_INCLUDE} ${RESOURCE_MANAGER_INCLUDES})


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions( -DEZ_LOG=4 )
    add_definitions( -DEZ_HAS_ASSERTIONS )
    enable_testing()
    add_subdirectory(__tests__)
endif()
add_subdirectory(GameDemo)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")