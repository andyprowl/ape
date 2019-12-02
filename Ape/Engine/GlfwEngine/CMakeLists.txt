project(GlfwEngine)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::Rendering::Rendering
    Ape::Rendering::Windowing)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::DearImGui
    3rdParty::Glad
    3rdParty::glfw
    Ape::Engine::Initialization
    Ape::Engine::UpdateHandling
    Basix::Container
    Basix::Profiling
    Basix::Time)

AddPrivateDefinitions(
    ${PROJECT_NAME}
    IMGUI_IMPL_OPENGL_LOADER_GLAD)
