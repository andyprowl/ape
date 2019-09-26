function(ImportGLAD)

    add_library(glad STATIC IMPORTED)

    set_target_properties(glad PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${glad_DIR}/include")

    set_target_properties(glad PROPERTIES IMPORTED_LOCATION "${glad_DIR}/lib/GLAD.lib")

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
    
    ImportGLAD()

    ImportGLFW()

endfunction()