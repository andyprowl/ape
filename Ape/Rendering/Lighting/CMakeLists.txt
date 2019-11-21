project(Lighting)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::World::Model
    Ape::World::Scene
    Basix::Range
    Glow::GpuResource
    Glow::Shader
    Glow::Texture)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Rendering::Culling
    Ape::World::Shape
    Basix::CompilerSupport
    Basix::Text)
