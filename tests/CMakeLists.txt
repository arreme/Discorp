cmake_minimum_required(VERSION 3.15)
project(CmakeTest)

add_executable(${PROJECT_NAME}
    src/catch_amalgamated.cpp
    src/inventory/test001.cpp
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
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ../src/commands/include
    include
    /usr/local/include/mongocxx/v_noabi
    /usr/local/include/libmongoc-1.0
    /usr/local/include/bsoncxx/v_noabi
    /usr/local/include/libbson-1.0
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)