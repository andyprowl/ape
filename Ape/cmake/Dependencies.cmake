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

endfunction()

function(ImportGlfw)

    find_package(glfw3 REQUIRED COMPONENTS glfw)

    if(glfw3_FOUND)
    
        message(" -- Glfw was found")
    
    else()
    
        message(" XX Glfw was not found!")
    
    endif()

endfunction()

function(ImportGlm)

    add_library(glm INTERFACE)

    set_target_properties(glm PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${glm_DIR}")

endfunction()

function(ImportDependencies)

    message("-- Importing external dependencies...")

    ImportAssimp()

    ImportGlfw()

    ImportGlm()

endfunction()