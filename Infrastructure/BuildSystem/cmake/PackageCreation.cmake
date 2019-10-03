function(
    CreatePackage
    PACKAGE_NAME)

    set(CURRENT_PACKAGE ${PACKAGE_NAME} PARENT_SCOPE)

    file(GLOB CMAKELISTS_FILE CMakeLists.txt)

endfunction()