project(InspectionOverlay)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::World::Scene)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::DearImGui
    3rdParty::Glad
    Ape::Engine::FrameProfiling
    Basix::Container
    Basix::Profiling
    Basix::Time)
