project(Model)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::World::Shape)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Basix::CompilerSupport)
