project(FrameProfiling)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Basix::Container
    Basix::Profiling
    Glow::Query)
