project(Engine)

AddLibrary(${PROJECT_NAME})

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::DearImGui
    3rdParty::Glad
    Ape::Engine::FrameProfiling
    Ape::Engine::Initialization
    Ape::Engine::InspectionOverlay
    Ape::Engine::Windowing
    Ape::World::Scene
    Basix::Container
    Basix::Profiling
    Basix::Signal
    Basix::Time)

AddPrivateDefinitions(
    ${PROJECT_NAME}
    IMGUI_IMPL_OPENGL_LOADER_GLAD)
