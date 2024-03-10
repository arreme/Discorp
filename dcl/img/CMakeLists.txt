cmake_minimum_required(VERSION 3.22)

project(dcl_img VERSION 0.0.1 DESCRIPTION "Image generation library wrapper for GD in C++")
add_library(${PROJECT_NAME} SHARED "src/renderer.cpp")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "include"
)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE dcl_core
    PRIVATE gd
    PRIVATE png
    PRIVATE z
    PRIVATE freetype
)

set(CMAKE_BUILD_TYPE Release)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
