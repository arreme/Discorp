cmake_minimum_required(VERSION 3.22)

project(dcl_db_handler VERSION 0.0.1 DESCRIPTION "The database entities for interacting with the database")
add_library(${PROJECT_NAME} SHARED "src/entities/player.cpp" "src/entities/user.cpp" "src/entities/interaction.cpp" "src/db_handler.cpp")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "include"
    PUBLIC "../db/include"
    PRIVATE "../util/include"
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC dcl_db
)

set(CMAKE_BUILD_TYPE Release)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

