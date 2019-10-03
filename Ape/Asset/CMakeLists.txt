project(Asset)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::GpuResource)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Foundational::CompilerSupport)
