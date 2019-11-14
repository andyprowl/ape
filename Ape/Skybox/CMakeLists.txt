project(Skybox)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Basix::Range
    Basix::Signal
    Glow::BufferObject
    Glow::GpuResource
    Glow::Shader)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Scene)
