project(Query)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Glow::GpuResource)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Basix::Memory)
