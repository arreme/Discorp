cmake_minimum_required(VERSION 3.22)

project(dcl_db_handler VERSION 0.0.1 DESCRIPTION "The database entities for interacting with the database")
add_library(${PROJECT_NAME} SHARED "src/db_user.cpp" "src/db_location.cpp" "src/db_inventory.cpp" "src/db_combat.cpp")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "include"
    PUBLIC "../db/include"
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE dcl_db
    PRIVATE dcl_core
)

set(CMAKE_BUILD_TYPE Release)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

