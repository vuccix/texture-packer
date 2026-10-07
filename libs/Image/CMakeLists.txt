add_library(Image STATIC src/Image.cpp)

target_include_directories(Image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
