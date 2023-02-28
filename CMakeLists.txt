cmake_minimum_required(VERSION 3.22)

option(DEBUG_MODE "Activate debug build" OFF)
option(BUILD_TESTS "Activate test project" OFF)

if(BUILD_TESTS)
    project(dcl_test VERSION 1.0.0 DESCRIPTION "Main Bot For DiscLand")
    add_executable(${PROJECT_NAME} test/setup.cpp test/db_test.cpp)
    find_package(Catch2 3 REQUIRED)
    find_package(bsoncxx 3.7.0 REQUIRED)
    find_package(mongocxx 3.7.0 REQUIRED)
    add_subdirectory(dcl/db)
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE Catch2::Catch2WithMain
        dcl_db
        bsoncxx
        mongocxx
        pthread
    )

    target_include_directories(
        ${PROJECT_NAME}
        PRIVATE "/usr/local/include/catch2" 
        PRIVATE "dcl/db/include"
        PRIVATE "/usr/local/include/mongocxx/v_noabi"
        PRIVATE "/usr/local/include/bsoncxx/v_noabi"
    )
else()

endif()

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(DEBUG_MODE)
    message(STATUS "DEBUG")
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()