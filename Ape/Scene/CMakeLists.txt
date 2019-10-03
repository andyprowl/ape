project(Scene)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    3rdParty::glm
    Ape::Asset
    Ape::GpuResource
    Foundational::Range
    Foundational::Signal)
    
AddPrivateDependencies(
    ${PROJECT_NAME}
    3rdParty::Glad)
    