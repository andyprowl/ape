project(Core)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} Asset glm Glow)

AddPrivateDependencies(${PROJECT_NAME} CompilerSupport Glad Stb)
