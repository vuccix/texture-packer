add_executable(TexturePacker WIN32)

target_sources(TexturePacker
        PRIVATE
        main.cpp
        TexturePacking.cpp
        View.cpp
)

find_package(OpenMP COMPONENTS C CXX REQUIRED)
target_link_options(TexturePacker PRIVATE
        "$<$<CXX_COMPILER_ID:GNU>:-fopenmp>"
)

# internal
target_link_libraries(TexturePacker
        PRIVATE
        TexturePacker::IO
        TexturePacker::Window
        ProjectWarnings
)

# external
target_link_libraries(TexturePacker
        PRIVATE
        glfw
        imgui
        OpenMP::OpenMP_CXX
)
