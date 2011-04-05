IF(WITH_X86_64)
    SET(MEX_DEF "-DX86_64")
ENDIF(WITH_X86_64)

IF(WITH_MATLAB_WRAP)
    SET(MEX_SRCS axon_read_adcinfo.cpp 
                 axon_read_dacinfo.cpp 
                 axon_read_data.cpp 
                 axon_read_epochinfo.cpp 
                 axon_read_epochinfoperdac.cpp 
                 axon_read_fileinfo.cpp 
                 axon_read_protocolinfo.cpp 
                 axon_read_strings.cpp 
                 axon_read_syncharray.cpp)
                 
    ADD_CUSTOM_TARGET(axonMex ALL 
                      DEPENDS axon
                      SOURCES ${MEX_SRCS})
    FOREACH(FN ${MEX_SRCS})
        ADD_CUSTOM_COMMAND(TARGET axonMex
                           DEPENDS axon
                           COMMAND ${MATLAB_MEX_EXECUTABLE} 
                                   -I${MATLAB_INCLUDE_DIR} 
                                   -I${axon_SOURCE_DIR}/src 
                                   ${MEX_DEF} 
                                   -L${axon_BINARY_DIR}/src
                                   -laxon
                                   ${CMAKE_CURRENT_SOURCE_DIR}/${FN}
                           COMMENT "Compiling ${FN}")
    ENDFOREACH(FN)
ENDIF(WITH_MATLAB_WRAP)
