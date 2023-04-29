cmake_minimum_required(VERSION 3.22)

project(dcl_core VERSION 0.0.1 DESCRIPTION "The core information of the dcl bot")
find_package(Protobuf REQUIRED)

add_library(${PROJECT_NAME} SHARED 
    "include/core/pb/item.pb.cc"
    "include/core/pb/location.pb.cc"
    "include/core/pb/map.pb.cc"
    "include/core/pb/player.pb.cc"
    "include/core/pb/interaction.pb.cc"
    "src/dcl_map.cpp"
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "include"
    PRIVATE ${PROTOBUF_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${PROTOBUF_LIBRARY}
)

set(CMAKE_BUILD_TYPE Release)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

