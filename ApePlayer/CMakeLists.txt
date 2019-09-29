project(ApePlayer)

AddExecutable(${PROJECT_NAME})

AddPrivateDependencies(${PROJECT_NAME} GlfwBinding TestScene)
