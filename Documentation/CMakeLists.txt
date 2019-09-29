project(Documentation)

file(GLOB_RECURSE DOC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/*.*")

CreateSourceGroup("docs" "Documentation Files" "${DOC_FILES}")
		
add_custom_target(${PROJECT_NAME} SOURCES ${DOC_FILES})
