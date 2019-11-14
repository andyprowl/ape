project(UpdateHandling)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Model
    Ape::Scene
    Basix::Range
    Glow::GpuResource)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Rendering
    Ape::Skybox
    Ape::Windowing
    Basix::CompilerSupport)
