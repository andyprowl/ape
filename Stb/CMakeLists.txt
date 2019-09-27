cmake_minimum_required(VERSION 3.10)

project(Stb)

file(GLOB HEADER_FILES include/stb/stb_image.h)

add_library(${PROJECT_NAME} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE include)
