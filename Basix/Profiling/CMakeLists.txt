project(Profiling)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Basix::Meta)

AddPrivateDependencies(
    ${PROJECT_NAME}
    Basix::Range)
