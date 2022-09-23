# minimum CMake version required
cmake_minimum_required(VERSION 3.15)
# Project name, version and description
project(Discorp VERSION 0.1 DESCRIPTION "Discorp")
 
# Add DPP as dependency
add_subdirectory(libs/DPP)
# You can also add any other libs you want to use
 
# Create an executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/commands/test1.cpp
    src/commands/send_image.cpp
)
 
# Linking libraries
target_link_libraries(${PROJECT_NAME}
    dpp
    gd
    png
    z
    freetype
    m
    pthread
)
 
# Specify includes
target_include_directories(${PROJECT_NAME} PRIVATE
    libs/DPP/include 
    libs/include
    src/commands/include
)
 
# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)