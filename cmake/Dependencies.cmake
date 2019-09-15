# Assimp's CMake configuration file does not create any target, so we create an imported one
# ourselves in order to be able to use target_link_libraries().
function(ImportAssimp)

    find_package(assimp REQUIRED)

    if(NOT assimp_FOUND)

        message(SEND_ERROR " XX Assimp was not found!")

        return()

    else()
    
        message(" -- Assimp was found")

    endif()

    add_library(assimp STATIC IMPORTED)

    set_target_properties(
        assimp
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES
        "${ASSIMP_INCLUDE_DIRS}")
    
    if(WIN32)

        set(ASSIMP_LIBRARY_EXTENSION ".lib")

    endif()

    set_target_properties(
        assimp
        PROPERTIES
        IMPORTED_LOCATION
        "${ASSIMP_LIBRARY_DIRS}/${ASSIMP_LIBRARIES}${ASSIMP_LIBRARY_EXTENSION}")

endfunction()

function(ImportGLFW)

    find_package(glfw3 REQUIRED COMPONENTS glfw)

    if(glfw3_FOUND)
    
        message(" -- GLFW was found")
    
    else()
    
        message(" XX GLFW was not found!")
    
    endif()

endfunction()

function(ImportDependencies)

    message("-- Importing external dependencies...")

    ImportAssimp()
    
    ImportGLFW()

endfunction()