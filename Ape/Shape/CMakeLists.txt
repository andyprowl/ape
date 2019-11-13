project(Shape)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::BufferObject
    Ape::Model
    Foundational::Range)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Foundational::CompilerSupport)
