cmake_minimum_required(VERSION 3.22)

project(dcl_db VERSION 0.0.1 DESCRIPTION "Database Handler library for DiscLand")
include_directories("include/")
add_library(${PROJECT_NAME} SHARED "src/db_write.cpp" "src/db_query_operations.cpp")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "/usr/local/include/mongocxx/v_noabi"
    PUBLIC "/usr/local/include/bsoncxx/v_noabi"
    PUBLIC "/include"
)

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC mongocxx
    PUBLIC bsoncxx
)


set(CMAKE_BUILD_TYPE Release)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
