add_library(tp_window)
add_library(TexturePacker::Window ALIAS tp_window)

target_sources(tp_window
        PRIVATE
        src/Window.cpp
)

target_include_directories(tp_window
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(tp_image
        PRIVATE
        ProjectWarnings
)

# external
target_link_libraries(tp_window
        PRIVATE
        glad
        glfw
)
