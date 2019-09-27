cmake_minimum_required(VERSION 3.10)

project(Stb)

file(
    GLOB
    CMAKE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.*"
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
    
file(GLOB HEADER_FILES include/stb/stb_image.h)

file(GLOB SOURCE_FILES src/stb_image.c)

source_group("CMake Files" FILES ${CMAKE_FILES})

add_library(${PROJECT_NAME} ${CMAKE_FILES} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
