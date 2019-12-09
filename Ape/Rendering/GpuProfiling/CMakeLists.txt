project(GpuProfiling)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Basix::Profiling
    Glow::Query)
