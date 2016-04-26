INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(pnglinkage SHARED pnglinkage.c)

TARGET_LINK_LIBRARIES(pnglinkage ${PNG_LIBRARY} ${ZLIB_LIBRARY})

SET(PYTHON_BINDINGS_SRC
    sip_pnglinkagepart0.c
)

ADD_CUSTOM_TARGET(copyPngLinkageInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${PYTHON_MODULES_FOLDER}/pnglinkage/__init__.py)

ADD_CUSTOM_COMMAND(OUTPUT ${PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} -j1 ${CMAKE_CURRENT_SOURCE_DIR}/pnglinkage.sip
                   COMMENT "Generating Python Bindings for pnglinkage"
                   DEPENDS pnglinkage.sip VERBATIM)

# Force to use SIP and headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

ADD_LIBRARY(pypnglinkage SHARED ${PYTHON_BINDINGS_SRC})

SET_TARGET_PROPERTIES(pypnglinkage PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULES_FOLDER}/pnglinkage)

ADD_DEPENDENCIES(pypnglinkage ${SIP_LIB})

SET_TARGET_PROPERTIES(pypnglinkage PROPERTIES OUTPUT_NAME pnglinkage PREFIX "_")

TARGET_LINK_LIBRARIES(pypnglinkage pnglinkage)

COPY_TARGET_LIBRARY_POST_BUILD(pnglinkage ${PYTHON_MODULES_FOLDER}/pnglinkage)
