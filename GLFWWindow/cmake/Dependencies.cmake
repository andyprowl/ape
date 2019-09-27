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

    ImportGLFW()

endfunction()