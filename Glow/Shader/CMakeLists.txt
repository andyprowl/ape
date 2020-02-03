project(Shader)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Basix::FileSystem
    Basix::Mathematics
    Basix::Meta
    Glow::GpuResource
    Glow::Texture)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    3rdParty::glm
    3rdParty::Stb
    Basix::CompilerSupport
    Basix::Range
    Glow::BufferObject)
