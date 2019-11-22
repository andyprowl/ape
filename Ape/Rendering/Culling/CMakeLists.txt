project(Culling)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::World::Scene)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad)
