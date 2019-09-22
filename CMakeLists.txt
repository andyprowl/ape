cmake_minimum_required(VERSION 3.10)

project(OpenGLTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include("cmake/CompilerDefinitions.cmake")

include("cmake/CompilerSettings.cmake")

include("cmake/Dependencies.cmake")

SetStrictWarningPolicy()

SetCpp17StandardCompliance()

ImportDependencies()

file(
    GLOB
    CMAKE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.*"
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

file(
    GLOB_RECURSE
    HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

file(
    GLOB
    SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(
    GLOB
    SHADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*.glsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*.glsi")

file(
    GLOB
    TEXTURE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/*.*")

file(
    GLOB
    DOC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/*.*")

source_group("CMake Files" FILES ${CMAKE_FILES})

source_group("Header Files" FILES ${HEADER_FILES})

source_group("Source Files" FILES ${SOURCE_FILES})

source_group("Resources\\Shader Files" FILES ${SHADER_FILES})

source_group("Resources\\Texture Files" FILES ${TEXTURE_FILES})

source_group("Documentation" FILES ${DOC_FILES})

add_executable(
    ${PROJECT_NAME}
    ${CMAKE_FILES}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${SHADER_FILES}
    ${TEXTURE_FILES}
    ${DOC_FILES})

AddPrivateDefinitions(shaderFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders")

AddPrivateDefinitions(textureFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources/textures")

AddPrivateDefinitions(modelFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources/models")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE assimp glad glfw glm stb)
