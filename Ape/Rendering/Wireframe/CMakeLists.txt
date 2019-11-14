project(Wireframe)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::World::Scene
    Ape::World::Shape
    Basix::Range
    Glow::GpuResource
    Glow::Shader)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad)
