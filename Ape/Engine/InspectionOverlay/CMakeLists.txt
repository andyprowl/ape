project(InspectionOverlay)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::Rendering::Rendering)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::DearImGui
    3rdParty::Glad
    Basix::Container
    Basix::Profiling
    Basix::Time)
