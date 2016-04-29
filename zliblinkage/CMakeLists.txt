INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Build the zliblinkage shared library
ADD_LIBRARY(zliblinkage SHARED zliblinkage.c)

# zliblinkage needs to be linked against zlib
TARGET_LINK_LIBRARIES(zliblinkage ${ZLIB_LIBRARY})

# Output the compiled library to the zliblinkage module folder once it has been built
SET_TARGET_PROPERTIES(zliblinkage PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULES_FOLDER}/zliblinkage)

# Generate bindings to it through the SIP tool
SET(PYTHON_BINDINGS_SRC
    sip_zliblinkagepart0.c
)
ADD_CUSTOM_COMMAND(OUTPUT ${PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} -j1 ${CMAKE_CURRENT_SOURCE_DIR}/zliblinkage.sip
                   COMMENT "Generating Python Bindings for zliblinkage"
                   DEPENDS zliblinkage.sip VERBATIM)

# Copy __init__.py for the zliblinkage module to its root folder
ADD_CUSTOM_TARGET(copyZlibLinkageInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${PYTHON_MODULES_FOLDER}/zliblinkage/__init__.py)
ADD_DEPENDENCIES(wheel copyZlibLinkageInitPy)

# Force to use SIP and headers located in the project
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

# Compile the _zliblinkage extension module
ADD_LIBRARY(pyzliblinkage SHARED ${PYTHON_BINDINGS_SRC})

# Set rpath of _zliblinkage to $ORIGIN in order for the libzliblinkage to be located
SET_TARGET_PROPERTIES(pyzliblinkage PROPERTIES INSTALL_RPATH "$ORIGIN")

# Output the compiled extension module to the zliblinkage module folder once it has been built
SET_TARGET_PROPERTIES(pyzliblinkage PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULES_FOLDER}/zliblinkage)

# Ensure sip has been built before compiling the extension module
ADD_DEPENDENCIES(pyzliblinkage ${SIP_LIB})

# Set the extension module name to _zliblinkage
SET_TARGET_PROPERTIES(pyzliblinkage PROPERTIES OUTPUT_NAME zliblinkage PREFIX "_")

# Link the extension module to zliblinkage
TARGET_LINK_LIBRARIES(pyzliblinkage zliblinkage)

ADD_DEPENDENCIES(wheel pyzliblinkage)
