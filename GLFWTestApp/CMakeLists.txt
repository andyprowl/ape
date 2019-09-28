project(GLFWTestApp)

AddExecutable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE GLFWWindow TestScene)
