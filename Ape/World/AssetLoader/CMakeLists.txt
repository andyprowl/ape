project(AssetLoader)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::assimp::assimp
    Ape::World::Model
    Ape::World::Shape
    Glow::Texture)
