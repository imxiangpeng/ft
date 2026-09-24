cmake_minimum_required(VERSION 3.16)

project(opencv_freetype_text LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)
find_package(Freetype REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
)

add_executable(opencv_freetype_text
    examples/main.cpp
    src/freetype_font.cpp
    src/text_layout.cpp
    src/opencv_text_renderer.cpp
)

target_link_libraries(opencv_freetype_text
    PRIVATE
        ${OpenCV_LIBS}
        ${FREETYPE_LIBRARIES}
)

set_target_properties(opencv_freetype_text PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
