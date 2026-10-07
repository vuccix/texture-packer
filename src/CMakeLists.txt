add_executable(TexturePacker
        main.cpp
        TexturePacking.cpp
        View.cpp
)

target_link_libraries(TexturePacker PRIVATE
        IO
        Window
        glfw
        imgui
        OpenMP::OpenMP_CXX
)
