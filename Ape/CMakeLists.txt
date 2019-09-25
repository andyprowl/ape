project(Ape)

include("cmake/Dependencies.cmake")

ImportDependencies()

file(
    GLOB
    DOC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/*.*")
    
file(
    GLOB
    CMAKE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.*"
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

file(
    GLOB_RECURSE
    PRIVATE_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private/*.hpp")

file(
    GLOB_RECURSE
    PUBLIC_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/public/*.hpp")

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

source_group("CMake Files" FILES ${CMAKE_FILES})

source_group("Header Files\\Private" FILES ${PRIVATE_HEADER_FILES})

source_group("Header Files\\Public" FILES ${PUBLIC_HEADER_FILES})

source_group("Source Files" FILES ${SOURCE_FILES})

source_group("Resources\\Shader Files" FILES ${SHADER_FILES})

source_group("Resources\\Texture Files" FILES ${TEXTURE_FILES})

source_group("Documentation" FILES ${DOC_FILES})

add_library(
    ${PROJECT_NAME}
    ${DOC_FILES}
    ${CMAKE_FILES}
    ${PRIVATE_HEADER_FILES}
    ${PUBLIC_HEADER_FILES}
    ${SOURCE_FILES}
    ${SHADER_FILES}
    ${TEXTURE_FILES})

AddPrivateDefinitions(shaderFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/private")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/public")

target_link_libraries(${PROJECT_NAME} PUBLIC assimp glm)

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw stb)

export(TARGETS ${EXPORTS} NAMESPACE Ape:: FILE ${PROJECT_NAME}Config.cmake)
