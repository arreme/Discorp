cmake_minimum_required(VERSION 3.22)

project(dcl_core VERSION 0.0.1 DESCRIPTION "Core library for DiscLand")
include_directories("include/")

FILE(GLOB_RECURSE SOURCE_LIST "src/*.cpp")

message(STATUS ${SOURCE_LIST})

add_library(${PROJECT_NAME} SHARED ${SOURCE_LIST})
target_link_libraries(${PROJECT_NAME} PRIVATE )