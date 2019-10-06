project(GlfwEngine)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::InputHandling
    Ape::Rendering)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    3rdParty::glfw
    Foundational::Time)
