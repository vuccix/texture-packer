add_library(tp_io)
add_library(TexturePacker::IO ALIAS tp_io)

target_sources(tp_io
        PRIVATE
        src/IO.cpp
)

target_include_directories(tp_io
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(tp_io
        PUBLIC
        TexturePacker::Image
        ProjectWarnings
)

# external
target_link_libraries(tp_io
        PRIVATE
        stb
        nfd
)
