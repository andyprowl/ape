function(
    AddPublicDependencies
    TARGET_NAME)

    target_link_libraries(${TARGET_NAME} PUBLIC ${ARGN})

endfunction()

function(
    AddInterfaceDependencies
    TARGET_NAME)

    target_link_libraries(${TARGET_NAME} INTERFACE ${ARGN})

endfunction()

function(
    AddPrivateDependencies
    TARGET_NAME)

    target_link_libraries(${TARGET_NAME} PRIVATE ${ARGN})

endfunction()
