cmake_minimum_required(VERSION 3.10)

project(GetBounds)

find_package(Nuke REQUIRED)

function(add_nuke_plugin PLUGIN_NAME)
	add_library(${PLUGIN_NAME} MODULE ${ARGN})
	add_library(NukePlugins::${PLUGIN_NAME} ALIAS ${PLUGIN_NAME})
	target_link_libraries(${PLUGIN_NAME} PRIVATE Nuke::NDK)
	target_compile_definitions(${PLUGIN_NAME} PRIVATE FN_EXAMPLE_PLUGIN)
	set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
	if (APPLE)
		set_target_properties(${PLUGIN_NAME} PROPERTIES SUFFIX ".dylib")
	endif()
endfunction()

add_nuke_plugin(GetBounds GetBounds.cpp)
