function(EnableIDEFolders)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER ${CMAKE_PREDEFINED_TARGETS_FOLDER})

endfunction()

function(
    SetProjectFolderForIDE
    TARGET_NAME
    PACKAGE_NAME)

    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)

    string(REPLACE "::" "/" PACKAGE_FOLDER "${PACKAGE_NAME}")

    if(TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")

        set_target_properties(
            ${TARGET_NAME}.Interface
            PROPERTIES
            FOLDER
            "${PACKAGE_FOLDER}")

    else()
    
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "${PACKAGE_FOLDER}")

    endif()
    
endfunction()
