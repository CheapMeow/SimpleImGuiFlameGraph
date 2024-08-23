cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
cmake_policy(SET CMP0076 NEW)

project(SimpleImGuiFireGraph VERSION 1.02)

option(IMGUI_FLAME_GRAPH_BUILD_EXAMPLE "Build an example using flame graph." ON)

list(APPEND IMGUI_FLAME_GRAPH_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/fire_graph_drawer.cpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/fire_graph_drawer.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/timer.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/scope_time_data.h>)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${IMGUI_FLAME_GRAPH_DIRECTORIES}>)
target_compile_definitions(${PROJECT_NAME} INTERFACE IMGUI_DEFINE_MATH_OPERATORS)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

if(IMGUI_FLAME_GRAPH_BUILD_EXAMPLE)
    add_subdirectory(example/)
endif()
