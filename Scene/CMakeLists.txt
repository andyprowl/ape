project(Scene)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} Asset glm Glow Range Signal)

AddPrivateDependencies(${PROJECT_NAME} Glad)
