project(Windowing)

AddLibrary(${PROJECT_NAME})

AddInterfaceDependencies(
    ${PROJECT_NAME}
    Foundational::Mathematics
    Foundational::Signal)
