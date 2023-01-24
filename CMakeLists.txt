# minimum CMake version required
cmake_minimum_required(VERSION 3.15)
# Project name, version and description
option(BUILD_TEST "You want to check if tests pass" OFF)
option(DEBUG_MODE "You want to create a debug build" OFF)

# list of exes that are shared from the two builds
set (COMMON_EXES
    "src/business/game_manager.cpp"
    "src/entities/user.cpp"
    "src/entities/player.cpp"
    "src/database/db_access.cpp"
    "src/database/db_handler.cpp"
    "src/database/db_write.cpp"
    "src/database/db_query.cpp"
    "src/entities/location.cpp"
)

set (COMMON_LIBS
    "gd"
    "png"
    "z"
    "freetype"
    "m"
    "pthread"
    "mongocxx"
    "bsoncxx"
)

set (COMMON_INCLUDES
    "/usr/local/include/mongocxx/v_noabi"
    "/usr/local/include/libmongoc-1.0"
    "/usr/local/include/bsoncxx/v_noabi"
    "/usr/local/include/libbson-1.0"
    "src/database/include"
    "src/business/include"
    "src/entities/include"
    "src/"
)

if(BUILD_TEST)
    project(DisTest VERSION 0.1 DESCRIPTION "Distest")
    add_subdirectory(libs/Catch2)
    add_executable(${PROJECT_NAME}
        ${COMMON_EXES}
        test/setup.cpp
        test/database.cpp
        test/dbhandler.cpp
        test/gametests.cpp
    )

    target_link_libraries(${PROJECT_NAME}
        ${COMMON_LIBS}
        Catch2WithMain
    )

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${COMMON_INCLUDES}
    )
    
    add_definitions( -DTEST_BUILD)
else()
    project(Discorp VERSION 0.1 DESCRIPTION "Discorp")

    add_subdirectory(libs/DPP)

    add_executable(${PROJECT_NAME}
        ${COMMON_EXES}
        src/main.cpp
        src/api/command_bootstrap.cpp
    )
    target_link_libraries(${PROJECT_NAME}
        ${COMMON_LIBS}
        dpp
    )

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${COMMON_INCLUDES}
        libs/DPP/include 
        src/api/include
    )
endif()

if(DEBUG_MODE)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)