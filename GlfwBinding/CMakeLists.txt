project(GlfwBinding)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} Core)

AddPrivateDependencies(${PROJECT_NAME} Glad glfw)
