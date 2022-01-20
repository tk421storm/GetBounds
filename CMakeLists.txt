cmake_minimum_required(VERSION 3.10)
set( CMAKE_C_COMPILER "gcc" )
set( CMAKE_CXX_COMPILER "g++" )
project(GetBounds VERSION 1.0.0)

#===------------------------------------------------------------------------===
# Global settings some based on the external configuration settings
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_CXX_VISIBILITY_PRESET hidden )
set( CMAKE_POSITION_INDEPENDENT_CODE True )
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif()

#===------------------------------------------------------------------------===
# Build information
string( TIMESTAMP BUILDDATE_YEAR_INTERNAL "%Y" )
string( TIMESTAMP BUILDDATE_MMDD_INTERNAL "%m%d" )
string( TIMESTAMP BUILDDATE_FULL_INTERNAL  "%Y-%m-%dT%H:%M:%S" )
string( TIMESTAMP BUILDDATE_STAMP "%Y.%m%d" )
string( REGEX REPLACE "^0" "" BUILDDATE_MMDD_INTERNAL ${BUILDDATE_MMDD_INTERNAL} )
set( BUILDDATE_YEAR "${BUILDDATE_YEAR_INTERNAL}" CACHE STRING "Year of the build: It will default to the current year." )
set( BUILDDATE_MMDD "${BUILDDATE_MMDD_INTERNAL}" CACHE STRING "Month and day of the build: It will default to the calendar month and day." )
set( BUILDDATE_FULL "${BUILDDATE_FULL_INTERNAL}" CACHE STRING "Exact time of the build." )

#===------------------------------------------------------------------------===

# Setting up sources and dependencies
set (SOURCES
        GetBounds.cpp
)
set (HEADERFILES
	Bounds.h
)

if(NOT NUKE_INSTALL_PATH)
    message(FATAL_ERROR "Nuke install path not set.")
endif()
find_library(DDIMAGE_LIBRARY NAME DDImage libDDImage PATHS ${NUKE_INSTALL_PATH} ${NUKE_INSTALL_PATH}/lib ${NUKE_INSTALL_PATH}/include)
if(NOT DDIMAGE_LIBRARY)
    message(FATAL_ERROR "DDImage library not found.")
endif()

# Create shared library
add_library(GetBounds SHARED
    ${SOURCES}
    ${HEADER_FILES}
)

target_link_libraries(GetBounds
	${DDIMAGE_LIBRARY}
)

target_include_directories(GetBounds PRIVATE ${NUKE_INSTALL_PATH}/include)
