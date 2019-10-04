project(Engine)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Asset
    Ape::GpuResource
    Ape::Scene)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Foundational::CompilerSupport)
    