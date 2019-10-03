project(ApePlayer)

AddExecutable(${PROJECT_NAME})

AddPrivateDependencies(
    ${PROJECT_NAME}
    Ape::GlfwEngine
    Application::TestScene)
