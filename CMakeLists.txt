cmake_minimum_required(VERSION 3.10)

# set CMAKE_MODULE_PATH to the directory containing FindNuke.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Nuke REQUIRED)

set (SOURCES
        GetBounds.cpp
)

# create your target
add_library(GetBounds SHARED 
  ${SOURCES}
)

# probably good idea to remove the prefix to leave target name as is on some platforms
set_target_properties(GetBounds PROPERTIES 
  PREFIX ""
)

# link your libraries
target_link_libraries(GetBounds PRIVATE 
  ${NUKE_LIBRARIES}
)

# install etc.

