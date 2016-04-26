INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

SET(siplib_SRCS
    apiversions.c
    array.c
    bool.cpp
    descriptors.c
    objmap.c
    qtlib.c
    siplib.c
    threads.c
    voidptr.c
    sip.h
)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/sip.h ${CMAKE_CURRENT_SOURCE_DIR}/siplib.c
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../configure.py --sip-module ${SIP_LIB}
                                                -b ${CMAKE_CURRENT_BINARY_DIR}/../sipgen
                                                -e ${CMAKE_CURRENT_SOURCE_DIR}/../siplib
                                                -d ${CMAKE_CURRENT_BINARY_DIR}/../siplib

                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sip.h.in ${CMAKE_CURRENT_SOURCE_DIR}/siplib.c.in
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ VERBATIM)

ADD_LIBRARY(${SIP_LIB} SHARED ${siplib_SRCS})

SET_TARGET_PROPERTIES(${SIP_LIB}
                      PROPERTIES OUTPUT_NAME ${SIP_LIB} PREFIX "")

SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULES_FOLDER})
