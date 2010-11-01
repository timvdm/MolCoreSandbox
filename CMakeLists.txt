cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

project(molcore)

# Version numbering (e.g. 0.1.0)
set(MC_MAJ_VER 0)
set(MC_MIN_VER 1)
set(MC_PATCH_VER 0)
set(MC_VERSION "${MC_MAJ_VER}.${MC_MIN_VER}.${MC_PATCH_VER}")

# Setup path handling
set(BIN_INSTALL_DIR      "bin"
  CACHE PATH "Install dir for binaries")
set(LIB_SUFFIX "" CACHE STRING "Suffix of the directory name, e.g. 64 for lib64")
if(UNIX)
  set(LIB_INSTALL_DIR "lib${LIB_SUFFIX}"
      CACHE PATH "Install dir for libraries")
  set(MC_PLUGIN_INSTALL_DIR "${LIB_INSTALL_DIR}/molcore" 
      CACHE PATH "Install dir for plugins")
else() # Windows - bin dir = lib dir to load libraries
  set(LIB_INSTALL_DIR "${BIN_INSTALL_DIR}"
      CACHE PATH "Install dir for libraries")
  if(MSVC)
    set(MC_PLUGIN_INSTALL_DIR "${BIN_INSTALL_DIR}"
        CACHE PATH "Install dir for plugins")
  else()
    set(MC_PLUGIN_INSTALL_DIR "lib${LIB_SUFFIX}/molcore"
        CACHE PATH "Install dir for plugins")
  endif()
endif()
set(MC_INCLUDE_DIRS "include/molcore")
set(MC_EXPORTS_FILE "${CMAKE_BINARY_DIR}/MolCore_EXPORTS.cmake")
# Ensure a fresh file is made each time CMake is run
file(REMOVE "${MC_EXPORTS_FILE}")

# Place binaries and libraries in the root of the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    CACHE STRING "Binary build output directory")
if(UNIX)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib${LIB_SUFFIX}"
      CACHE STRING "Library build output directory")
else() # This is preferable on WIndows for example - bin = lib directory.
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      CACHE STRING "Library build output directory")
endif()

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


set(libmolcore_hdrs
    fileformat.h
    traits.h
    plugin.h
    detail/pluginloader.h
    detail/pluginfactory.h
    molecule.h 
    abstractmolecule.h
    moleculemodels.h
   )

set(libmolcore_srcs
    src/detail/unixpluginloader.cpp
   )


# Set up the include directory and symlink/copy all headers to it.
# Note: Windows lacks symlinks and so the files must be copied.
make_directory("${molcore_BINARY_DIR}/include/molcore")
make_directory("${molcore_BINARY_DIR}/include/molcore/detail")
#make_directory("${molcore_BINARY_DIR}/include/molcore/molecule")
if(UNIX)
  set(header_cmd "create_symlink")
else()
  set(header_cmd "copy_if_different")
endif()
foreach(headerFile ${libmolcore_hdrs})
  set(from "${CMAKE_CURRENT_SOURCE_DIR}/src/${headerFile}")
  set(to   "${molcore_BINARY_DIR}/include/molcore/${headerFile}")
  execute_process(
      COMMAND ${CMAKE_COMMAND} -E ${header_cmd} "${from}" "${to}")
endforeach()


enable_testing()


add_library(molcore SHARED ${libmolcore_srcs})

add_library(smilesformat SHARED src/smilesformat.cxx)



set(tests
    smilesformat
   )

foreach(test ${tests})
  add_executable(${test}_test test/${test}.cpp)
  target_link_libraries(${test}_test molcore dl)
  add_test(${test}_test ${test}_test)
endforeach()
