cmake_minimum_required(VERSION 3.22)

project(dcl_api VERSION 0.0.1 DESCRIPTION "DPP wrapper to manage actions from discord")

add_library(${PROJECT_NAME} SHARED "src/bootstraps.cpp" "src/request.cpp")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "include"
    PUBLIC "/usr/local/include/dpp"
    PRIVATE "/usr/local/include/mongocxx/v_noabi"
    PRIVATE "/usr/local/include/bsoncxx/v_noabi"
)

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC "/usr/local/lib/libdpp.so"
    PRIVATE dcl_core
    PRIVATE dcl_db_handler
    PRIVATE dcl_img
)

set(CMAKE_BUILD_TYPE Release)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
