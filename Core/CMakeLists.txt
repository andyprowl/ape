project(Core)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} assimp::assimp glm)

AddPrivateDependencies(${PROJECT_NAME} Glad Stb)
