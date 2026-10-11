# OpenGL ===============================================================================================================
find_package(OpenGL REQUIRED)

# glad =================================================================================================================
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

## glfw =================================================================================================================
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

# miniz ================================================================================================================
add_library(miniz STATIC miniz/miniz/miniz.c)
target_include_directories(miniz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/miniz)

# stb ==================================================================================================================
add_library(stb STATIC stb/stb/stb_impl.cpp)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)
target_link_libraries(stb PRIVATE miniz)

# imgui ================================================================================================================
file(GLOB IMGUI_SRC imgui/ImGui/*.cpp)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(imgui
        PRIVATE
        glfw
        glad
        OpenGL::GL
)

# native file dialog - extended ========================================================================================
set(NFD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_INSTALL     OFF CACHE BOOL "" FORCE)

add_subdirectory(nfd)
