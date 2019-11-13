project(Rendering)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Effect
    Ape::GpuResource
    Ape::Lighting
    Ape::Model
    Ape::Scene
    Ape::Shape
    Ape::Skybox
    Ape::Wireframe
    Foundational::Range)

AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad
    Ape::Windowing
    Foundational::CompilerSupport
    Foundational::Text)
