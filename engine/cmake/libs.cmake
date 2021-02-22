configure_file(cmake/fmt.in library/fmt-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/library/fmt-download )
if(result)
    message(FATAL_ERROR "CMake step for fmt failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/library/fmt-download )
if(result)
    message(FATAL_ERROR "Build step for fmt failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/library/fmt-src ${CMAKE_CURRENT_BINARY_DIR}/library/fmt-build
        EXCLUDE_FROM_ALL)

set(LIBS_CURRENT fmt)
set(INCLUDE_CURRENT ${CMAKE_CURRENT_BINARY_DIR}/library/fmt-src/include)