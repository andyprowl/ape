project(GpuResource)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Foundational::Mathematics)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    3rdParty::glm
    3rdParty::Stb
    Foundational::CompilerSupport)
