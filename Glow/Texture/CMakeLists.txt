project(Texture)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Basix::FileSystem
    Basix::Mathematics
    Basix::Meta
    Glow::GpuResource)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    3rdParty::glm
    3rdParty::Stb
    Basix::CompilerSupport)
