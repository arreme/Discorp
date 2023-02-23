cmake_minimum_required(VERSION 3.22)
project(dcl_bot VERSION 1.0.0 DESCRIPTION "Main Bot For DiscLand")

option(DEBUG_MODE "Activate debug build" OFF)

if(DEBUG_MODE)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(${PROJECT_NAME} main2.cpp)
add_subdirectory(core)
target_include_directories(${PROJECT_NAME} PRIVATE "core/include/")

target_link_libraries(${PROJECT_NAME} dcl_core)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)


