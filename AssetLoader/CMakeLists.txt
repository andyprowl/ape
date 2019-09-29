project(AssetLoader)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} assimp::assimp Core)
