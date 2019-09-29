file(GLOB_RECURSE DOC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/*.*")

CreateSourceGroup("docs" "Documentation Files" "${DOC_FILES}")
		
add_custom_target(Documentation SOURCES ${DOC_FILES})
