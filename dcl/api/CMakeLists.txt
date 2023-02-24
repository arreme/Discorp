cmake_minimum_required(VERSION 3.22)

list(APPEND LINK_DIRS "core/include/" PARENT_SCOPE)

project(dcl_core VERSION 0.0.1 DESCRIPTION "Core library for DiscLand")
include_directories("include/")
add_library(${PROJECT_NAME} SHARED "src/core_func.cpp")