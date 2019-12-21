project(GlfwEngine)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::Engine::Engine
    Ape::Engine::Initialization
    Ape::Engine::Windowing)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::DearImGui
    3rdParty::Glad
    3rdParty::glfw)
