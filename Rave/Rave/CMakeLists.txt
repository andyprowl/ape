project(Rave)

AddExecutable(${PROJECT_NAME})

AddPrivateDependencies(
    ${PROJECT_NAME}
    Ape::GlfwEngine
    Rave::RaveCore)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    VS_DEBUGGER_COMMAND_ARGUMENTS
    "--enable-debug-output")
