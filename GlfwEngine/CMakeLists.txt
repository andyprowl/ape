project(GlfwEngine)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} Engine)

AddPrivateDependencies(${PROJECT_NAME} Glad glfw)
