cmake_minimum_required(VERSION 4.1)
project(TexturePacker LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD    26)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wpedantic -Wextra -Werror)
add_compile_options(-g -O3 -fopenmp)
add_link_options(-fopenmp)

find_package(OpenMP REQUIRED)

add_subdirectory(external)
add_subdirectory(src)
