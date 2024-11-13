cmake_minimum_required(VERSION 3.0.0)

project(Todo_List VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    "src/*.h"
    "src/*.cpp"
    "src/*.ixx"
    )

add_executable(Todo_List ${SOURCES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Todo_List)