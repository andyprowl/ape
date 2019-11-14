function(
    CreatePackage
    PACKAGE_NAME)

    if (CURRENT_PACKAGE)

        set(SEPARATOR "::")

    endif()

    set(CURRENT_PACKAGE "${CURRENT_PACKAGE}${SEPARATOR}${PACKAGE_NAME}" PARENT_SCOPE)

    file(GLOB CMAKELISTS_FILE CMakeLists.txt)

endfunction()