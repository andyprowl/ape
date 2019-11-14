project(Effect)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Effect
    Basix::Range
    Basix::Signal
    Glow::BufferObject
    Glow::Shader)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Basix::Text)
