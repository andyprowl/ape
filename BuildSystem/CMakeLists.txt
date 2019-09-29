project(BuildSystem)

file(
    GLOB_RECURSE
    CMAKE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.*")

file(GLOB CMAKELISTS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

CreateSourceGroup("cmake" "CMake Files" "${CMAKE_FILES}")

add_custom_target(${PROJECT_NAME} SOURCES ${CMAKE_FILES} ${CMAKELISTS_FILE})
