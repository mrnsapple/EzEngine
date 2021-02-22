set(APP_TEST_SCENE App/Scene/Scene.cpp
    ../../App/Scene/Private/SceneManager.cpp
    ../../App/Scene/Private/Scene.cpp
    ../../App/Scene/Public/Scene.hpp
    ../../App/Scene/Public/SceneManager.hpp
    ../../GameObjects/Private/SceneComponent.cpp
    ../../GameObjects/Private/BaseComponent.cpp



    )

set(CORE_TEST_TIME Core/Time/time.cpp Core/Time/clock.cpp
        ../../Core/Time/Private/Clock.cpp ../../Core/Time/Private/Time.cpp)

set(APP_TEST ${APP_TEST_SCENE})
