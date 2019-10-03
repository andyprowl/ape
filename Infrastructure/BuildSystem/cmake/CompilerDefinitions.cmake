function(
    AddPublicDefinitions
    TARGET_NAME
    SYMBOLS)

    target_compile_definitions(${TARGET_NAME} PUBLIC ${SYMBOLS})

endfunction()

function(
    AddInterfaceDefinitions
    TARGET_NAME
    SYMBOLS)

    target_compile_definitions(${TARGET_NAME} INTERFACE ${SYMBOLS})

endfunction()

function(
    AddPrivateDefinitions
    TARGET_NAME
    SYMBOLS)

    target_compile_definitions(${TARGET_NAME} PRIVATE ${SYMBOLS})

endfunction()
