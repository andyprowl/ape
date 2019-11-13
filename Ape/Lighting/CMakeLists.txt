project(Lighting)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::GpuResource
    Ape::Model
    Ape::Scene
    Ape::Shader
    Ape::Texture
    Foundational::Range)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Shape
    Ape::Windowing
    Foundational::CompilerSupport
    Foundational::Text)
