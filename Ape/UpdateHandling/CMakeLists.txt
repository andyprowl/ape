project(UpdateHandling)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::GpuResource
    Ape::Model
    Ape::Scene
    Foundational::Range)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Foundational::CompilerSupport)
