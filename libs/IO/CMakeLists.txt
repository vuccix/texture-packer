add_library(IO STATIC src/IO.cpp)

target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(IO
        PUBLIC
        Image
        PRIVATE
        stb
        nfd
)
