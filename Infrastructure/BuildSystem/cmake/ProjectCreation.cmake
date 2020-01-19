include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/CompilerDefinitions.cmake)

include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/ProjectFolder.cmake)

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
    IMPLEMENTATION_FILES
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
        ${IMPLEMENTATION_FILES}
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
    IMPLEMENTATION_FILES
    RESOURCE_FILES)

    CreateSourceGroup("cmake" "CMake Files" "${CMAKE_FILES}")

    CreateSourceGroup("docs" "Documentation Files" "${DOC_FILES}")

    CreateSourceGroup("include/public" "Header Files/Public" "${PUBLIC_HEADER_FILES}")

    CreateSourceGroup("include/private" "Header Files/Private" "${PRIVATE_HEADER_FILES}")

    CreateSourceGroup("src" "Source Files" "${IMPLEMENTATION_FILES}")

    CreateSourceGroup("resources" "Resource Files" "${RESOURCE_FILES}")

endfunction()

function(
    GetProjectFilesAndCreateGroups
    PROJECT_FILES
    HAS_IMPLEMENTATION_FILES)

    GetProjectFiles(
        CMAKE_FILES
        DOC_FILES
        PUBLIC_HEADER_FILES
        PRIVATE_HEADER_FILES
        IMPLEMENTATION_FILES
        RESOURCE_FILES)

    CreateSourceGroups(
        "${CMAKE_FILES}"
        "${DOC_FILES}"
        "${PUBLIC_HEADER_FILES}"
        "${PRIVATE_HEADER_FILES}"
        "${IMPLEMENTATION_FILES}"
        "${RESOURCE_FILES}")

    set(${PROJECT_FILES}
        "${CMAKE_FILES}"
        "${DOC_FILES}"
        "${PUBLIC_HEADER_FILES}"
        "${PRIVATE_HEADER_FILES}"
        "${IMPLEMENTATION_FILES}"
        "${RESOURCE_FILES}"
        PARENT_SCOPE)

    if(IMPLEMENTATION_FILES)

        set(${HAS_IMPLEMENTATION_FILES} TRUE PARENT_SCOPE)

    else()

        set(${HAS_IMPLEMENTATION_FILES} FALSE PARENT_SCOPE)

    endif()

endfunction()

function(
    AddLocalIncludeDirectories
    TARGET_NAME
    HAS_IMPLEMENTATION_FILES)

    if(HAS_IMPLEMENTATION_FILES)

        target_include_directories(
            ${TARGET_NAME}
            PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include/public")

        target_include_directories(
            ${TARGET_NAME}
            PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/include/private")

    else()

        target_include_directories(
            ${TARGET_NAME}
            INTERFACE
            "${CMAKE_CURRENT_SOURCE_DIR}/include/public")

    endif()

endfunction()

function(
    AddLibraryTarget
    TARGET_NAME
    PROJECT_FILES
    HAS_IMPLEMENTATION_FILES)

    if(HAS_IMPLEMENTATION_FILES)

        message("-- Adding static library target for project ${TARGET_NAME}")

        add_library(${TARGET_NAME} STATIC ${PROJECT_FILES})

    else()

        message("-- Adding header-only library target for project ${TARGET_NAME}")

        add_library(${TARGET_NAME} INTERFACE)

        add_custom_target(${TARGET_NAME}.Interface SOURCES ${PROJECT_FILES})

        set_property(TARGET ${TARGET_NAME}.Interface PROPERTY PROJECT_LABEL ${TARGET_NAME})

        set_target_properties(${TARGET_NAME}.Interface PROPERTIES LINKER_LANGUAGE CXX)

    endif()

    set(QUALIFIED_ALIAS "${CURRENT_PACKAGE}::${TARGET_NAME}")

    add_library(${QUALIFIED_ALIAS} ALIAS ${TARGET_NAME})

endfunction()

function(
    AddExecutableTarget
    TARGET_NAME
    PROJECT_FILES)

    message("-- Adding executable target for project ${TARGET_NAME}")

    add_executable(${TARGET_NAME} ${PROJECT_FILES})

    if(${CURRENT_PACKAGE})

        set(QUALIFIED_ALIAS ${CURRENT_PACKAGE}::${TARGET_NAME})

        add_executable(${QUALIFIED_ALIAS} ALIAS ${TARGET_NAME})

    endif()

endfunction()

function(
    AddTarget
    TARGET_NAME
    TARGET_TYPE)

    GetProjectFilesAndCreateGroups(PROJECT_FILES HAS_IMPLEMENTATION_FILES)
    
    if("${TARGET_TYPE}" STREQUAL "LIBRARY")
    
        AddLibraryTarget(${TARGET_NAME} "${PROJECT_FILES}" ${HAS_IMPLEMENTATION_FILES})

    else()

        AddExecutableTarget(${TARGET_NAME} "${PROJECT_FILES}")

    endif()

    AddLocalIncludeDirectories(${PROJECT_NAME} ${HAS_IMPLEMENTATION_FILES})

    if(HAS_IMPLEMENTATION_FILES)

        AddPrivateDefinitions(
            ${TARGET_NAME} 
            resourceFolder="${CMAKE_CURRENT_SOURCE_DIR}/resources")

        set(PUBLIC_RESOURCE_FOLDER resourceFolder::_${TARGET_NAME})

        if(CURRENT_PACKAGE)

            set(PUBLIC_RESOURCE_FOLDER "resourceFolder::${CURRENT_PACKAGE}::${TARGET_NAME}")

        endif()

        string(REPLACE "::" "_" PUBLIC_RESOURCE_FOLDER "${PUBLIC_RESOURCE_FOLDER}")

        AddPublicDefinitions(
            ${TARGET_NAME} 
            ${PUBLIC_RESOURCE_FOLDER}="${CMAKE_CURRENT_SOURCE_DIR}/resources")

    endif()

    SetProjectFolderForIDE(${TARGET_NAME} ${CURRENT_PACKAGE})

    export(TARGETS ${EXPORTS} NAMESPACE ${CURRENT_PACKAGE} FILE ${TARGET_NAME}Config.cmake)

endfunction()

function(
    AddLibrary
    LIBRARY_NAME)

    AddTarget(${LIBRARY_NAME} "LIBRARY")

endfunction()

function(
    AddExecutable
    EXECUTABLE_NAME)

    AddTarget(${EXECUTABLE_NAME} "EXECUTABLE")

endfunction()
