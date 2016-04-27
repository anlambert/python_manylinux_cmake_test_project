INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(zliblinkage SHARED zliblinkage.c)

TARGET_LINK_LIBRARIES(zliblinkage ${ZLIB_LIBRARY})

SET(PYTHON_BINDINGS_SRC
    sip_zliblinkagepart0.c
)

ADD_CUSTOM_TARGET(copyZlibLinkageInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${PYTHON_MODULES_FOLDER}/zliblinkage/__init__.py)

ADD_CUSTOM_COMMAND(OUTPUT ${PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} -j1 ${CMAKE_CURRENT_SOURCE_DIR}/zliblinkage.sip
                   COMMENT "Generating Python Bindings for zliblinkage"
                   DEPENDS zliblinkage.sip VERBATIM)

# Force to use SIP and headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

ADD_LIBRARY(pyzliblinkage SHARED ${PYTHON_BINDINGS_SRC})
SET_TARGET_PROPERTIES(pyzliblinkage PROPERTIES INSTALL_RPATH "$ORIGIN")

SET_TARGET_PROPERTIES(pyzliblinkage PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULES_FOLDER}/zliblinkage)

ADD_DEPENDENCIES(pyzliblinkage ${SIP_LIB})

SET_TARGET_PROPERTIES(pyzliblinkage PROPERTIES OUTPUT_NAME zliblinkage PREFIX "_")

TARGET_LINK_LIBRARIES(pyzliblinkage zliblinkage)

COPY_TARGET_LIBRARY_POST_BUILD(zliblinkage ${PYTHON_MODULES_FOLDER}/zliblinkage)
