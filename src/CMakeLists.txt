add_executable(TexturePacker
        main.cpp
        TexturePacking.cpp
)

target_link_libraries(TexturePacker PRIVATE
        stb
        OpenMP::OpenMP_CXX
)
