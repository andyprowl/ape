project(UpdateHandling)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Engine::Engine
    Ape::World::Model
    Ape::World::Scene
    Basix::Range
    Glow::GpuResource)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Engine::Windowing
    Ape::Rendering::Rendering
    Ape::Rendering::Skybox
    Basix::CompilerSupport)
