project(Skybox)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::BufferObject
    Ape::GpuResource
    Ape::Shader
    Foundational::Range
    Foundational::Signal)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Scene)
