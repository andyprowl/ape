project(Engine)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} Asset glm Glow Scene)

AddPrivateDependencies(${PROJECT_NAME} CompilerSupport Glad)
