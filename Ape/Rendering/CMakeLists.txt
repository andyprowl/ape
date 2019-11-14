project(Rendering)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Effect
    Ape::Lighting
    Ape::Model
    Ape::Scene
    Ape::Shape
    Ape::Skybox
    Ape::Wireframe
    Glow::BufferObject
    Glow::Texture
    Basix::Range)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Windowing
    Basix::CompilerSupport
    Basix::Text)
