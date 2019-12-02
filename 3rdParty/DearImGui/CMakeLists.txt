cmake_minimum_required(VERSION 3.10)

project(DearImGui)

AddLibrary(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE "include/public/DearImGui")
