cmake_minimum_required(VERSION 3.10)

project(OpenGLTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(glfw3 COMPONENTS glfw)

file(
    GLOB_RECURSE
    APP_PRIVATE_INCLUDE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private/*.hpp")

file(
    GLOB
    APP_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(
    GLOB
    APP_SHADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl")

file(
    GLOB
    APP_TEXTURE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/textures/*.*")

file(
    GLOB_RECURSE
    GLAD_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private/glad/*.h")

file(
    GLOB_RECURSE
    GLAD_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

file(
    GLOB_RECURSE
    STB_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private/stb/*.h")

file(
    GLOB_RECURSE
    STB_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stbimage.cpp")

source_group("App\\Source Files" FILES ${APP_SOURCE_FILES})

source_group("App\\Shader Files" FILES ${APP_SHADER_FILES})

source_group("App\\Texture Files" FILES ${APP_TEXTURE_FILES})

source_group("App\\Private Header Files" FILES ${APP_PRIVATE_INCLUDE_FILES})

source_group("GLAD\\Header Files" FILES ${GLAD_HEADER_FILES})

source_group("GLAD\\Source Files" FILES ${GLAD_SOURCE_FILES})

source_group("STB\\Header Files" FILES ${STB_HEADER_FILES})

source_group("STB\\Source Files" FILES ${STB_SOURCE_FILES})

add_executable(
    OpenGLTest
    ${APP_PRIVATE_INCLUDE_FILES}
    ${APP_SOURCE_FILES}
    ${APP_SHADER_FILES}
    ${APP_TEXTURE_FILES}
    ${GLAD_HEADER_FILES}
    ${GLAD_SOURCE_FILES}
    ${STB_HEADER_FILES}
    ${STB_SOURCE_FILES})

target_compile_definitions(OpenGLTest PRIVATE shaderFolder="${CMAKE_CURRENT_SOURCE_DIR}/shaders")

target_compile_definitions(OpenGLTest PRIVATE textureFolder="${CMAKE_CURRENT_SOURCE_DIR}/textures")

target_include_directories(OpenGLTest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/private")

target_include_directories(OpenGLTest PRIVATE ${GLM_DIR})

target_link_libraries(OpenGLTest PRIVATE glfw)
