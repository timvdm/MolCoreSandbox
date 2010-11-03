add_library(smilesformat SHARED smilesformat.cxx)
set_target_properties(smilesformat PROPERTIES 
                      PREFIX ""
                      SUFFIX ".dyn")


install(TARGETS smilesformat
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${MC_PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${MC_PLUGIN_INSTALL_DIR})
 


