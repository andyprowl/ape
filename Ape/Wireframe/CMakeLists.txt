project(Wireframe)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::GpuResource
    Ape::Scene
    Ape::Shader
    Ape::Shape
    Foundational::Range)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad)
