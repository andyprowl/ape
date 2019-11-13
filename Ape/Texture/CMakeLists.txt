project(Texture)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::GpuResource
    Foundational::FileSystem
    Foundational::Mathematics
    Foundational::Meta)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    3rdParty::glm
    3rdParty::Stb
    Foundational::CompilerSupport)
