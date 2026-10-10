add_library(tp_image)
add_library(TexturePacker::Image ALIAS tp_image)

target_sources(tp_image
        PRIVATE
        src/Image.cpp
)

target_include_directories(tp_image
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(tp_image
        PRIVATE
        ProjectWarnings
)
