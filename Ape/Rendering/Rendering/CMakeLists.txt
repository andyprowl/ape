project(Rendering)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Engine::Engine
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
    Ape::Engine::FrameProfiling
    Ape::Engine::Windowing
    Basix::CompilerSupport
    Basix::Text)
