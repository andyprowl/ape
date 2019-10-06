project(TestScene)

file(
    GLOB_RECURSE
    MODEL_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/models/*.*")

AddLibrary(${PROJECT_NAME})

# The following prevents .obj model files to be interpreted as object files to be linked.
set_source_files_properties(${MODEL_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::AssetLoader
    Ape::InputHandling
    Ape::Rendering
    Ape::UpdateHandling
    Foundational::CompilerSupport)
