add_library(mylib mylib.cpp)
add_library(Task Task.cpp)

target_include_directories(mylib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(Task
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

        

