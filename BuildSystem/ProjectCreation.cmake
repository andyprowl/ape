include(${SOURCE_ROOT}/BuildSystem/CompilerDefinitions.cmake)

macro(
    FindFilesRecursively
    RESULT)

    file(GLOB_RECURSE FOUND_FILES ${ARGN})

    set(${RESULT} ${FOUND_FILES} PARENT_SCOPE)

endmacro()

function(
    GetProjectFiles
    CMAKE_FILES
    DOC_FILES
    PUBLIC_HEADER_FILES
    PRIVATE_HEADER_FILES
    SOURCE_FILES
    RESOURCE_FILES)

    FindFilesRecursively(
        ${CMAKE_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.*")

    FindFilesRecursively(
        ${DOC_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}/docs/*.*")

    FindFilesRecursively(
        ${PUBLIC_HEADER_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}/include/public/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/public/*.hpp")

    FindFilesRecursively(
        ${PRIVATE_HEADER_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}/include/private/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/private/*.hpp")

    FindFilesRecursively(
        ${SOURCE_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

    FindFilesRecursively(
        ${RESOURCE_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.*")

endfunction()

macro(
    CreateSourceGroup
    ROOT
    GROUP_NAME
    GROUP_FILES)

    source_group(
        TREE
        "${CMAKE_CURRENT_SOURCE_DIR}/${ROOT}"
        PREFIX
        ${GROUP_NAME}
        FILES
        ${GROUP_FILES})

endmacro()

function(
    CreateSourceGroups
    CMAKE_FILES
    DOC_FILES
    PUBLIC_HEADER_FILES
    PRIVATE_HEADER_FILES
    SOURCE_FILES
    RESOURCE_FILES)

    CreateSourceGroup("cmake" "CMake Files" "${CMAKE_FILES}")

    CreateSourceGroup("docs" "Documentation Files" "${DOC_FILES}")
        
    CreateSourceGroup("include/public" "Header Files/Public" "${PUBLIC_HEADER_FILES}")
        
    CreateSourceGroup("include/private" "Header Files/Private" "${PRIVATE_HEADER_FILES}")

    CreateSourceGroup("src" "Source Files" "${SOURCE_FILES}")

    CreateSourceGroup("resources" "Resource Files" "${RESOURCE_FILES}")

endfunction()

function(
    AddLibrary
    LIBRARY_NAME)

    GetProjectFiles(
        CMAKE_FILES
        DOC_FILES
        PUBLIC_HEADER_FILES
        PRIVATE_HEADER_FILES
        SOURCE_FILES
        RESOURCE_FILES)

    CreateSourceGroups(
        "${CMAKE_FILES}"
        "${DOC_FILES}"
        "${PUBLIC_HEADER_FILES}"
        "${PRIVATE_HEADER_FILES}"
        "${SOURCE_FILES}"
        "${RESOURCE_FILES}")

    add_library(
        ${LIBRARY_NAME}
        ${CMAKE_FILES}
        ${DOC_FILES}
        ${PUBLIC_HEADER_FILES}
        ${PRIVATE_HEADER_FILES}
        ${SOURCE_FILES}
        ${RESOURCE_FILES})

    target_include_directories(
        ${LIBRARY_NAME}
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/public")

    target_include_directories(
        ${LIBRARY_NAME}
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/private")

    AddPrivateDefinitions(
        ${LIBRARY_NAME} 
        resourceFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources")

endfunction()

function(
    AddExecutable
    EXECUTABLE_NAME)

    GetProjectFiles(
        CMAKE_FILES
        DOC_FILES
        PUBLIC_HEADER_FILES
        PRIVATE_HEADER_FILES
        SOURCE_FILES
        RESOURCE_FILES)

    CreateSourceGroups(
        "${CMAKE_FILES}"
        "${DOC_FILES}"
        "${PUBLIC_HEADER_FILES}"
        "${PRIVATE_HEADER_FILES}"
        "${SOURCE_FILES}"
        "${RESOURCE_FILES}")

    add_executable(
        ${EXECUTABLE_NAME}
        ${CMAKE_FILES}
        ${DOC_FILES}
        ${PUBLIC_HEADER_FILES}
        ${PRIVATE_HEADER_FILES}
        ${SOURCE_FILES}
        ${RESOURCE_FILES})

    target_include_directories(
        ${EXECUTABLE_NAME}
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/public")

    target_include_directories(
        ${EXECUTABLE_NAME}
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/private")

    AddPrivateDefinitions(
        ${EXECUTABLE_NAME} 
        resourceFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources")

endfunction()
