#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(B1)

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
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cc)
list(REMOVE_ITEM ${sources} lightguides.cc)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(lightguides ${PROJECT_SOURCE_DIR}/src/lightguides.cc ${sources} ${headers})
target_link_libraries(lightguides ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all macros to the build directory
#
file(GLOB_RECURSE macros CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/macros/*.mac)
file(COPY macros DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS lightguides DESTINATION bin)
