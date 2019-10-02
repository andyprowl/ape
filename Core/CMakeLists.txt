project(Core)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} glm Shader)

AddPrivateDependencies(${PROJECT_NAME} CompilerSupport Glad Stb)
