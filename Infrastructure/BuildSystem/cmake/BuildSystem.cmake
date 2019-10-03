include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/CompilerDefinitions.cmake)

include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/CompilerSettings.cmake)

include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/PackageCreation.cmake)

include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/ProjectCreation.cmake)

include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/ProjectFolder.cmake)

include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/TargetAlias.cmake)

include(${SOURCE_ROOT}/Infrastructure/BuildSystem/cmake/TargetDependencies.cmake)

macro(InitializeBuildSystem)

    include(${SOURCE_ROOT}/3rdParty/External/cmake/Dependencies.cmake)

    EnableIDEFolders()

    ImportExternalDependencies()

endmacro()
