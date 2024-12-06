
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
  cmake_policy(SET CMP0071 NEW)
endif()

PROJECT(IMGResample)

set(DEFAULT_BUILD_TYPE "Release")

if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release"
       CACHE STRING
       "Semicolon separated list of supported configuration types, only supports debug and release, anything else will be ignored"
       FORCE)

  set_property(CACHE CMAKE_CONFIGURATION_TYPES
                PROPERTY STRINGS
                "Debug" "Release")
endif()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message (STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set (CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING
       "Choose the type of build." FORCE)
endif()

set(BUILD_SHARED_LIBS NO)

# First for the generic no-config case (e.g. with mingw)
include(GNUInstallDirs)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

find_package(VTK COMPONENTS 
  CommonCore
  CommonDataModel
  CommonMisc
  IOImage
  ImagingCore  
)

if(NOT VTK_FOUND)
  message(FATAL_ERROR "Unable to find the VTK build folder.")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB CXX_FILES *.cxx)

message("cxx files ${CXX_FILES}")
# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")

add_executable(IMGResample ${CXX_FILES} )


target_link_libraries(IMGResample
	${VTK_LIBRARIES}
)

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS IMGResample
  MODULES ${VTK_LIBRARIES}
)
