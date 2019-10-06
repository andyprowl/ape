project(Model)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::GpuResource
    Foundational::Meta)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Foundational::CompilerSupport)
