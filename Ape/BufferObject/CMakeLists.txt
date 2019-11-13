project(BufferObject)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::GpuResource
    Foundational::Mathematics
    Foundational::Meta)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    3rdParty::glm
    Ape::Texture)
