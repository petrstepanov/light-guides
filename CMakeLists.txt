#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(LightGuides)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate SOURCES and HEADERS for this project
# NB: HEADERS are included so they will show up in IDEs
#
# set(MAIN_CPP "${PROJECT_SOURCE_DIR}/src/lightguides.cc")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cc)
message (STATUS "Found list of sources: ${SOURCES}")
#list(REMOVE_ITEM ${SOURCES} ${MAIN_CPP})

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.hh)
message (STATUS "Found list of headers: ${HEADERS}")

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
set(EXECUTABLE_TARGET ${PROJECT_NAME}-bin)
add_executable(${EXECUTABLE_TARGET} ${SOURCES} ${HEADERS})
target_link_libraries(${EXECUTABLE_TARGET} ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Set executable output name
#
#get_filename_component(EXECUTABLE_NAME "${MAIN_CPP}" NAME_WE)
get_filename_component(EXECUTABLE_NAME "${PROJECT_NAME}" NAME_WE)
string(TOLOWER "${EXECUTABLE_NAME}" EXECUTABLE_NAME)
set_property(TARGET ${EXECUTABLE_TARGET} PROPERTY OUTPUT_NAME ${EXECUTABLE_NAME})

#----------------------------------------------------------------------------
# Copy all macros to the build directory (at build time, not build system generation)
#
add_custom_target(copy_macros
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/macros ${CMAKE_CURRENT_BINARY_DIR}/macros
)
add_dependencies(${EXECUTABLE_TARGET} copy_macros)

#----------------------------------------------------------------------------
# Copy all SLURM scripts to the build directory (at build time, not build system generation)
#
add_custom_target(copy_slurm
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/slurm ${CMAKE_CURRENT_BINARY_DIR}/slurm
)
add_dependencies(${EXECUTABLE_TARGET} copy_slurm)

#----------------------------------------------------------------------------
# Create output directory (at build time, not build system generation) since Geant4 cannot create directories
#
add_custom_target(output_dir
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/output
)
add_dependencies(${EXECUTABLE_TARGET} output_dir)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ${EXECUTABLE_TARGET} DESTINATION bin)

#----------------------------------------------------------------------------
# Build the draw scripts
#
add_subdirectory (draw)