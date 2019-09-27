cmake_minimum_required(VERSION 3.10)

project(Glad)

file(
    GLOB
    CMAKE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.*"
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

file(GLOB SOURCE_FILES src/glad.c)

file(GLOB_RECURSE HEADER_FILES include/*.h)

source_group("CMake Files" FILES ${CMAKE_FILES})

source_group(
    TREE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PREFIX
    "Header Files"
    FILES
    ${HEADER_FILES})

source_group("Source Files" FILES ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${CMAKE_FILES} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
