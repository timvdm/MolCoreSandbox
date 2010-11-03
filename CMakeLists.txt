cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

project(molcore)

# Version numbering (e.g. 0.1.0)
set(MC_MAJ_VER 0)
set(MC_MIN_VER 1)
set(MC_PATCH_VER 0)
set(MC_VERSION "${MC_MAJ_VER}.${MC_MIN_VER}.${MC_PATCH_VER}")



#
# Find dependencies
#
find_package(Boost COMPONENTS filesystem)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})








# Setup path handling
set(BIN_INSTALL_DIR       "bin" 
    CACHE PATH "Install dir for binaries")
set(LIB_SUFFIX            "" 
    CACHE STRING "Suffix of the directory name, e.g. 64 for lib64")
set(LIB_INSTALL_DIR       "lib${LIB_SUFFIX}" 
    CACHE PATH "Install dir for libraries")
set(MC_PLUGIN_INSTALL_DIR "${LIB_INSTALL_DIR}/molcore" 
    CACHE PATH "Install dir for plugins")

# Place binaries and libraries in the root of the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    CACHE STRING "Binary build output directory")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib${LIB_SUFFIX}"
    CACHE STRING "Library build output directory")

mark_as_advanced(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  CMAKE_RUNTIME_OUTPUT_DIRECTORY)



# Ensure the MolCore include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
    "${molcore_BINARY_DIR}/include;${tmp_include_dirs}")
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${EIGEN2_INCLUDE_DIR}
   )

enable_testing()



add_subdirectory(libmolcore)
add_subdirectory(plugins)
add_subdirectory(test)

