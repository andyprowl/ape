project(Rendering)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Rendering::Effect
    Ape::Rendering::Lighting
    Ape::Rendering::Skybox
    Ape::Rendering::Wireframe
    Ape::World::Model
    Ape::World::Scene
    Ape::World::Shape
    Glow::BufferObject
    Glow::Texture
    Basix::Range)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Rendering::GpuProfiling
    Ape::Rendering::Windowing
    Basix::CompilerSupport
    Basix::Text)
