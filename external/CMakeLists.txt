# OpenGL ===============================================================================================================
find_package(OpenGL REQUIRED)

# stb ==================================================================================================================
add_library(stb STATIC stb/stb/stb_impl.cpp)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)
set_target_properties(stb PROPERTIES COMPILE_WARNING_AS_ERROR OFF)
target_compile_options(stb PRIVATE -w)

# native file dialog - extended ========================================================================================
set(NFD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_INSTALL     OFF CACHE BOOL "" FORCE)

add_subdirectory(nfd)
