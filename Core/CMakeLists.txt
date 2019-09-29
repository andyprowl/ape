project(Core)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} glm)

AddPrivateDependencies(${PROJECT_NAME} Glad Stb)
