project(Glow)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} Mathematics)

AddPrivateDependencies(${PROJECT_NAME} CompilerSupport Glad glm Stb)
