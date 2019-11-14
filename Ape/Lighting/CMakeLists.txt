project(Lighting)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Model
    Ape::Scene
    Basix::Range
    Glow::GpuResource
    Glow::Shader
    Glow::Texture)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Shape
    Ape::Windowing
    Basix::CompilerSupport
    Basix::Text)
