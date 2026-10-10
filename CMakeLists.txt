cmake_minimum_required(VERSION 4.1)
project(TexturePacker LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS       OFF)

set(CMAKE_C_STANDARD            11)
set(CMAKE_C_STANDARD_REQUIRED   ON)
set(CMAKE_C_EXTENSIONS         OFF)

include(ProjectWarnings.cmake)

add_subdirectory(external)
add_subdirectory(libs)
add_subdirectory(src)
