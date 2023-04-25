cmake_minimum_required(VERSION 3.22)

project(dcl_api VERSION 0.0.1 DESCRIPTION "DPP wrapper to manage actions from discord")

add_library(${PROJECT_NAME} SHARED "src/command_bootstrap.cpp" "src/button_bootstrap.cpp")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "include"
    PUBLIC "/usr/local/include/dpp"
)

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC dpp
    PUBLIC dcl_core
    PUBLIC db_handler
)

set(CMAKE_BUILD_TYPE Release)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
