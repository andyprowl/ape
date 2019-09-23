project(TestApp)

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
    GLOB_RECURSE
    MODEL_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/models/*.*")

source_group("CMake Files" FILES ${CMAKE_FILES})

source_group("Header Files" FILES ${HEADER_FILES})

source_group("Source Files" FILES ${SOURCE_FILES})

source_group("Resources\\Shader Files" FILES ${SHADER_FILES})

source_group("Resources\\Texture Files" FILES ${TEXTURE_FILES})

source_group(
    TREE
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/models"
    PREFIX
    "Resources\\Model Files"
    FILES
    ${MODEL_FILES})

source_group("Documentation" FILES ${DOC_FILES})

add_executable(
    ${PROJECT_NAME}
    ${DOC_FILES}
    ${CMAKE_FILES}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${SHADER_FILES}
    ${TEXTURE_FILES}
    ${MODEL_FILES})

AddPrivateDefinitions(shaderFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders")

AddPrivateDefinitions(textureFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources/textures")

AddPrivateDefinitions(modelFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources/models")

# The following prevents .obj model files to be interpreted as object files to be linked.
set_source_files_properties(${MODEL_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE TestScene)
