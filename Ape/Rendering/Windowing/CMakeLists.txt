project(Windowing)

AddLibrary(${PROJECT_NAME})

AddInterfaceDependencies(
    ${PROJECT_NAME}
    Basix::Mathematics
    Basix::Signal)
