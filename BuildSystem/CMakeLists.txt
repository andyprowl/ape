project(BuildSystem)

file(GLOB CMAKE_SOURCE_FILES *.cmake)

add_custom_target(${PROJECT_NAME} SOURCES ${CMAKE_SOURCE_FILES})

get_property(IDE_FOLDER GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER)

source_group("CMake Files" FILES ${CMAKE_SOURCE_FILES})

if(IDE_FOLDER)

    SetProjectFolderForIDE(${IDE_FOLDER})

endif()
