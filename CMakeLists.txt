cmake_minimum_required(VERSION 3.20)
project(TexturePacker LANGUAGES CXX C)

if(APPLE)
    enable_language(OBJC)
    enable_language(OBJCXX)
endif()

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS       OFF)

set(CMAKE_C_STANDARD            11)
set(CMAKE_C_STANDARD_REQUIRED   ON)
set(CMAKE_C_EXTENSIONS         OFF)

include(ProjectWarnings.cmake)

add_subdirectory(external)
add_subdirectory(libs)
add_subdirectory(src)
