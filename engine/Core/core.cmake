set(CORE_EXCEPTION
        Core/Exception/Private/base.cpp
        Core/Exception/Private/out_of_bound.cpp
        Core/Exception/Private/out_of_range.cpp
        Core/Exception/Private/ThreadPool_is_not_available.cpp
        Core/Exception/Private/out_of_range.cpp
        Core/Exception/Private/fileIOFailed.cpp
        Core/Exception/Private/division_by_zero.cpp
        Core/Exception/Private/manager_not_found.cpp
        Core/Exception/Private/renderer_exceptions.cpp)

set(CORE_FILEIO
        Core/FileIO/Private/FileIO.cpp
        Core/FileIO/Public/FileIO.hpp)

set(CORE_LOG
        Core/Log/log.hpp)

set(CORE_TIME
        Core/Time/Private/Time.cpp
        Core/Time/Private/Clock.cpp)

set(CORE_MEMORY
        Core/MemoryManagement/Private/Allocator.cpp
        Core/MemoryManagement/Private/DoubleBufferAllocator.cpp
        Core/MemoryManagement/Private/PoolAllocator.cpp
        Core/MemoryManagement/Private/PoolManager.cpp
        Core/MemoryManagement/Private/StackAllocator.cpp)
    
set(CORE_THREADPOOL
        Core/ThreadPool/Private/ThreadSequentialMechanics.cpp
        Core/ThreadPool/Private/Thread.cpp
        Core/ThreadPool/Private/ThreadParallelMechanics.cpp
        Core/ThreadPool/Private/ThreadPoolInternal.cpp
        Core/ThreadPool/Private/ThreadPool.cpp)

set(CORE_EVENTS
        Core/Events/Private/Event.cpp
        Core/Events/Private/AudioEvent.cpp
        Core/Events/Private/KeyboardEvent.cpp)

set(CORE_UNIQUE_ID
        Core/UniqueID/Private/UniqueID.cpp
        Core/UniqueID/Private/UniqueIDManager.cpp
        Core/UniqueID/Private/WeakUniqueID.cpp
        Core/UniqueID/Private/WeakNodeID.cpp)

set(CORE_MATH
        Core/Math/Public/Vector.hpp
        Core/Math/Public/Matrix.hpp
        Core/Math/Public/HashingOperators.hpp)

set(CORE_INIT
        Core/Initializing/Private/Init.cpp)

if (UNIX)
    set(CORE_RENDERER_LIBDEP X11 pthread vulkan dl Xxf86vm Xrandr Xi)
elseif (WIN32)
    set(CORE_RENDERER_LIBDEP vulkan-1)
endif()

set(CORE_WINDOW
        Core/Window/Private/Window.cpp
        Core/Window/Private/IWindow.cpp
        Core/Window/Private/XWindow.cpp
        Core/Window/Private/WNDWindow.cpp)

set(CORE_RENDERER
        Core/Renderer/Private/Model.cpp
        Core/Renderer/Private/Renderer.cpp
        Core/Renderer/Private/VisualInstance.cpp)

set(CORE
        ${CORE_EXCEPTION}
        ${CORE_MEMORY}
        ${CORE_INIT}
        ${CORE_TIME}
        ${CORE_MATH}
        ${CORE_WINDOW}
        ${CORE_RENDERER}
        ${CORE_EVENTS}
        ${CORE_UNIQUE_ID}
        ${CORE_THREADPOOL}
        ${CORE_FILEIO})

set(CORE_INCLUDE
    Core/Log
    Core/Exception/Public
    Core/MemoryManagement/Public
    Core/Time/Public
    Core/Initializing/Public
    Core/Math/Public
    Core/UniqueID/Public
    Core/Events/Public
    Core/ThreadPool/Public
    Core/FileIO/Public
    Core/Utility
    Core/FileIO/Public
    Core/Window/Public
    Core/Renderer/Public)

    
