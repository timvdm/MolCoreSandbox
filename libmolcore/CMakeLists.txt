
set(libmolcore_concept_hdrs
    concepts/atom.h
   )

set(libmolcore_detail_hdrs
    detail/pluginfactory.h
    detail/pluginloader.h
   )

set(libmolcore_hdrs
    foreach.h
    traits.h
    plugin.h
    pluginmanager.h
    fileformat.h
    models/molecule.h 
    models/abstractmolecule.h
    models/moleculemodels.h
    models/dietzmolecule.h
    ${libmolcore_concept_hdrs}
    ${libmolcore_detail_hdrs}
   )

set(libmolcore_srcs
    src/detail/unixpluginloader.cpp
    src/pluginmanager.cpp
   )


# Set up the include directory and symlink/copy all headers to it.
# Note: Windows lacks symlinks and so the files must be copied.
make_directory("${molcore_BINARY_DIR}/include/molcore")
make_directory("${molcore_BINARY_DIR}/include/molcore/detail")
make_directory("${molcore_BINARY_DIR}/include/molcore/concepts")
make_directory("${molcore_BINARY_DIR}/include/molcore/models")
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


add_library(molcore SHARED ${libmolcore_srcs})
target_link_libraries(molcore ${Boost_LIBRARIES})
