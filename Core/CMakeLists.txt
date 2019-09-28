project(Core)

AddLibrary(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC assimp::assimp glm)

target_link_libraries(${PROJECT_NAME} PRIVATE Glad Stb)

export(TARGETS ${EXPORTS} NAMESPACE Ape:: FILE ${PROJECT_NAME}Config.cmake)
