cmake_minimum_required(VERSION 3.22)

set(CMAKE_BUILD_TYPE Release)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

project(dcl_core VERSION 0.0.1 DESCRIPTION "The core machinery for discland")
add_library(${PROJECT_NAME} SHARED "src/db_write.cpp" "src/db_query_operations.cpp")

include_directories(
    PUBLIC "/usr/local/include/mongocxx/v_noabi"
    PUBLIC "/usr/local/include/bsoncxx/v_noabi"
    PUBLIC "/include"
    PUBLIC "../db/include"
)

link_libraries(
    PUBLIC mongocxx
    PUBLIC bsoncxx
    PUBLIC dcl_db
)

