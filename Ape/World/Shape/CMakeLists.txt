project(Shape)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::World::Model
    Basix::Range
    Glow::BufferObject)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Basix::CompilerSupport)
