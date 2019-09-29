project(GLFWTestApp)

AddExecutable(${PROJECT_NAME})

AddPrivateDependencies(${PROJECT_NAME} GLFWWindow TestScene)
