# Assimp's CMake configuration file does not create any target, so we create an imported one
# ourselves in order to be able to use target_link_libraries().
macro(ImportAssimp)

    find_package(assimp REQUIRED)

    if(NOT assimp_FOUND)

        message(SEND_ERROR " XX Assimp was not found!")

        return()

    else()
    
        message(" -- Assimp was found")

        AddLibraryAlias(3rdParty::assimp::assimp assimp::assimp)

    endif()

endmacro()

macro(ImportGlfw)

    find_package(glfw3 REQUIRED COMPONENTS glfw)

    if(glfw3_FOUND)
    
        message(" -- GLFW was found")
    
        AddLibraryAlias(3rdParty::glfw glfw)
    
    else()
    
        message(" XX GLFW was not found!")
    
    endif()

endmacro()

macro(ImportGlm)

    add_library(glm INTERFACE)
    
    if(glm_DIR)

        file(GLOB GLM_HEADER "${glm_DIR}/glm/glm.hpp")

        if(NOT "${GLM_HEADER}" STREQUAL "")
        
            set(glm_FOUND TRUE)

        endif()

    endif()

    if(glm_FOUND)

        message(" -- GLM was found")

        set_target_properties(glm PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${glm_DIR}")

        add_library(3rdParty::glm ALIAS glm)

    else()

        message(SEND_ERROR " XX Missing *required* dependency: The GLM library was not found!")

    endif()

endmacro()

macro(ImportQt)

    find_package(Qt5 COMPONENTS Core OpenGL Widgets)

    if(Qt5_FOUND)
    
        message(" -- Qt5 was found")

        AddLibraryAlias(3rdParty::Qt5::Core Qt5::Core)
        
        AddLibraryAlias(3rdParty::Qt5::OpenGL Qt5::OpenGL)

        AddLibraryAlias(3rdParty::Qt5::Widgets Qt5::Widgets)
    
    else()
    
        message(" XX Qt5 was not found!")
    
    endif()

endmacro()

macro(ImportExternalDependencies)

    message("-- Importing external 3rd-party dependencies...")

    ImportAssimp()

    ImportGlfw()

    ImportGlm()

    ImportQt()

endmacro()