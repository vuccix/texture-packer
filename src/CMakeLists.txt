add_executable(TexturePacker
        main.cpp
        TexturePacking.cpp
        IO.cpp
)

target_link_libraries(TexturePacker PRIVATE
        stb
        nfd
        imgui
        glfw
        glad
        OpenMP::OpenMP_CXX
)
