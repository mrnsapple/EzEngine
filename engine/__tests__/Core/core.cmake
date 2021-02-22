set(CORE_TEST_LOG
        Core/Log/log.cpp)

set(FILEIO_LOG
        Core/FileIO/FileIO.cpp)

set(CORE_TEST_TIME
        Core/Time/time.cpp
        Core/Time/clock.cpp
        ../../Core/Time/Private/Clock.cpp
        ../../Core/Time/Private/Time.cpp)
    
set(CORE_TEST_MEMORY
        Core/MemoryTest/DoubleBufferAllocator.cpp
        Core/MemoryTest/StackAllocatorTest.cpp
        Core/MemoryTest/PoolAllocatorTest.cpp
        Core/MemoryTest/Actor.cpp
        ../../Core/MemoryManagement/Private/Allocator.cpp
        ../../Core/MemoryManagement/Private/DoubleBufferAllocator.cpp
        ../../Core/MemoryManagement/Private/PoolAllocator.cpp
        ../../Core/MemoryManagement/Private/PoolManager.cpp
        ../../Core/MemoryManagement/Private/StackAllocator.cpp
       )

set(CORE_TEST_THREADPOOL
        ../../Core/ThreadPool/Private/ThreadParallelMechanics.cpp
        ../../Core/ThreadPool/Private/ThreadSequentialMechanics.cpp
        ../../Core/ThreadPool/Private/Thread.cpp
        Core/ThreadPool/ThreadPool.cpp
        ../../Core/ThreadPool/Private/ThreadPool.cpp
        ../../Core/ThreadPool/Private/ThreadPoolInternal.cpp
        Core/ThreadPool/ThreadSequentialMechanics.cpp
        Core/ThreadPool/ThreadParallelMechanics.cpp)

set(CORE_TEST_MATH
        Core/Math/matrix.cpp
        Core/Math/vector.cpp)

set(CORE_TEST_UNIQUEID
        Core/UniqueId/uniqueId.cpp
        ../../Core/UniqueID/Private/UniqueIDManager.cpp
        ../../Core/UniqueID/Private/UniqueID.cpp)

set(CORE_EVENTS
        Core/EventTests/EventTest.cpp
        ../../Core/Events/Private/Event.cpp)

set(CORE_TEST_RENDERER
        ../../Core/Renderer/Private/Model.cpp
        ../../Core/Renderer/Private/Renderer.cpp
        ../../Core/Renderer/Private/VisualInstance.cpp
        Core/RendererTests/RendererTest.cpp)

set(CORE_TEST_WINDOW
        ../../Core/Window/Private/Window.cpp
        ../../Core/Window/Private/IWindow.cpp
        ../../Core/Window/Private/XWindow.cpp
        ../../Core/Window/Private/WNDWindow.cpp
        Core/WindowTests/WindowTest.cpp)    

set(CORE_VISUALTEST
        Core/VisualTest/VIT_main.cpp
)

set(ALL_EXCEPTIONS
        ../../Core/Exception/Private/base.cpp
        ../../Core/Exception/Private/manager_not_found.cpp
        ../../Core/Exception/Private/out_of_bound.cpp
        ../../Core/Exception/Private/out_of_range.cpp
        ../../Core/Exception/Private/division_by_zero.cpp
        ../../Core/Exception/Private/ThreadPool_is_not_available.cpp
        ../../Core/Exception/Private/renderer_exceptions.cpp)


set(CORE_TEST
        ${ALL_EXCEPTIONS}
        ${CORE_TEST_MEMORY}
        ${CORE_TEST_TIME}
        ${CORE_EVENTS}
        ${CORE_TEST_UNIQUEID}
        ${CORE_TEST_MATH}
        ${CORE_TEST_THREADPOOL}
        ${FILEIO_LOG}
        #${CORE_TEST_WINDOW}
        ${CORE_TEST_RENDERER}
        #${CORE_VISUALTEST}
        )


if (UNIX)
    set(CORE_TEST_RENDERER_LIBDEP X11 pthread vulkan dl Xxf86vm Xrandr Xi)
elseif (WIN32)
    set(CORE_TEST_RENDERER_LIBDEP vulkan-1)
endif()
