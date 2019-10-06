project(Initialization)

AddLibrary(${PROJECT_NAME})

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad)