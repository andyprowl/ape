cmake_minimum_required(VERSION 3.10)

project(Glad)

file(GLOB SOURCE_FILES src/glad.c)

file(GLOB HEADER_FILES include/glad.h)

source_group("Header Files" FILES ${HEADER_FILES})

source_group("Source Files" FILES ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
