cmake_minimum_required(VERSION 3.15)

project (CH3-96-2_GUI)

set (TARGET_NAME CH3-96-2_GUI)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")         # Установка флага линковки /MT

add_subdirectory(Panel)
add_subdirectory(Saver)
add_subdirectory(checksum)
add_subdirectory(Upgrader)
