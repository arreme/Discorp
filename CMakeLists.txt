cmake_minimum_required(VERSION 3.22)

option(DEBUG_MODE "Activate debug build" OFF)
option(BUILD_TESTS "Activate test project" OFF)

if(BUILD_TESTS)
    project(dcl_test VERSION 1.0.0 DESCRIPTION "Main Test executable")
    add_executable(${PROJECT_NAME} 
        test/setup.cpp 
        test/db_handler_test.cpp
        test/proto_test.cpp
        test/core_test.cpp
        dcl/img/src/gd_io_stream.cpp
    )
    add_definitions(-DTEST_DATABASE)
    find_package(Catch2 3 REQUIRED)
    add_subdirectory(dcl/db)
    add_subdirectory(dcl/db_handler)
    add_subdirectory(dcl/core)
    add_subdirectory(dcl/img)
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE Catch2::Catch2WithMain
        dcl_db
        dcl_db_handler
        dcl_core
        dcl_img
        protobuf
        pthread
    )

    target_include_directories(
        ${PROJECT_NAME}
        PRIVATE "/usr/local/include/catch2" 
        PRIVATE "dcl/db/include"
        PRIVATE "dcl/db_handler/include"
        PRIVATE "dcl/core/include"
        PRIVATE "dcl/img/include"
    )
else()
    project(dcl_main VERSION 1.0.0 DESCRIPTION "Main Bot For DiscLand")
    add_executable(${PROJECT_NAME} "dcl/main.cpp")
    add_subdirectory(libs/DPP)
    add_subdirectory(dcl/db)
    add_subdirectory(dcl/db_handler)
    add_subdirectory(dcl/core)
    add_subdirectory(dcl/img)
    add_subdirectory(dcl/api)

    target_link_libraries(
        ${PROJECT_NAME}
        dpp
        dcl_api
        dcl_db
        dcl_db_handler
        dcl_core
        dcl_img
        pthread
    )

    target_include_directories(
        ${PROJECT_NAME}
        PRIVATE "libs/DPP/include"
        PRIVATE "dcl/api/include"
    )

endif()

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

if(DEBUG_MODE)
    message(STATUS "DEBUG")
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()