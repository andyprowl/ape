project(GlfwEngine)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::Rendering
    Ape::Windowing)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    3rdParty::glfw
    Ape::Initialization
    Ape::UpdateHandling
    Foundational::Time)
