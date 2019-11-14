project(UpdateHandling)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::World::Model
    Ape::World::Scene
    Basix::Range
    Glow::GpuResource)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Rendering::Rendering
    Ape::Rendering::Skybox
    Ape::Rendering::Windowing
    Basix::CompilerSupport)
