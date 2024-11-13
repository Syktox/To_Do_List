cmake_minimum_required(VERSION 3.0.0)

project(Todo_List VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME} mylib)
target_link_libraries(${PROJECT_NAME} Task)
target_link_libraries(${PROJECT_NAME} TodoList)