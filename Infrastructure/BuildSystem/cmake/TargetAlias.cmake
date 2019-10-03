# Unlike the add_library(... ALIAS ...) command, this works also for IMPORTED targets that are
# not globally visible
function(
    AddLibraryAlias
    ALIAS_NAME
    TARGET_NAME)

    add_library(${ALIAS_NAME} INTERFACE IMPORTED)
    
    target_link_libraries(${ALIAS_NAME} INTERFACE ${TARGET_NAME})

endfunction()
