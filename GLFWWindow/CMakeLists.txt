project(GLFWWindow)

AddLibrary(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC Core)

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)

export(TARGETS ${EXPORTS} NAMESPACE Ape:: FILE ${PROJECT_NAME}Config.cmake)
