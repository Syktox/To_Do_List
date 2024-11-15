cmake_minimum_required(VERSION 3.5.0)

project(Todo_List VERSION 1.0 LANGUAGES CXX)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Todo_List)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets CONFIG REQUIRED)
	
add_executable(Todo_List WIN32 ${SOURCES})

file(GLOB SOURCES
    "src/*.h"
    "src/*.cpp"
    "src/*.ixx"
    )


target_include_directories(Todo_List PRIVATE
    "header"
    ${CMAKE_INSTALL_PREFIX}/include
    )
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
target_sources(Todo_List PRIVATE ${SOURCES})
target_compile_options(Todo_List PRIVATE /W4 /WX)

target_link_libraries(Todo_List PRIVATE
	wx::core wx::base
    )
	
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
target_sources(Todo_List PRIVATE ${SOURCES})
target_compile_definitions(Todo_List PRIVATE
    "NOMINMAX"
    "_WIN32_WINNT=_WIN32_WINNT_WIN7"
    "WIN32_LEAN_AND_MEAN"
    "VC_EXTRALEAN"
    )