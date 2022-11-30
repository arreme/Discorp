# minimum CMake version required
cmake_minimum_required(VERSION 3.15)
# Project name, version and description
option(BUILD_TEST "You want to check if tests pass" OFF)

if(BUILD_TEST)
    project(DisTest VERSION 0.1 DESCRIPTION "Distest")
    add_subdirectory(libs/Catch2)
    add_executable(${PROJECT_NAME}
        test/test.cpp
    )

    target_link_libraries(${PROJECT_NAME}
        gd
        png
        z
        freetype
        m
        pthread
        mongocxx
        bsoncxx
        Catch2WithMain
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        /usr/local/include/mongocxx/v_noabi
        /usr/local/include/libmongoc-1.0
        /usr/local/include/bsoncxx/v_noabi
        /usr/local/include/libbson-1.0
    )   
else()
    project(Discorp VERSION 0.1 DESCRIPTION "Discorp")

    add_subdirectory(libs/DPP)

    add_executable(${PROJECT_NAME}
        src/main.cpp
        src/api/command_bootstrap.cpp
    )
    target_link_libraries(${PROJECT_NAME}
        dpp
        gd
        png
        z
        freetype
        m
        pthread
        mongocxx
        bsoncxx
    )

    target_include_directories(${PROJECT_NAME} PRIVATE
        libs/DPP/include 
        src/api/include
        src/entities
        /usr/local/include/mongocxx/v_noabi
        /usr/local/include/libmongoc-1.0
        /usr/local/include/bsoncxx/v_noabi
        /usr/local/include/libbson-1.0
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)