project(Core)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} glm Glow)

AddPrivateDependencies(${PROJECT_NAME} CompilerSupport Glad Stb)
