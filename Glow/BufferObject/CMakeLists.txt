project(BufferObject)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Basix::Mathematics
    Basix::Meta
    Glow::GpuResource)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    3rdParty::glm
    Glow::Texture)
