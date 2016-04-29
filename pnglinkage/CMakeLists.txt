INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Build the pnglinkage shared library
ADD_LIBRARY(pnglinkage SHARED pnglinkage.c)

# pnglinkage needs to be linked against libpng
TARGET_LINK_LIBRARIES(pnglinkage ${PNG_LIBRARY})

# Output the compiled library to the pnlinkage module folder once it has been built
SET_TARGET_PROPERTIES(pnglinkage PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULES_FOLDER}/pnglinkage)

# Generate bindings to it through the SIP tool
SET(PYTHON_BINDINGS_SRC
    sip_pnglinkagepart0.c
)

ADD_CUSTOM_COMMAND(OUTPUT ${PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} -j1 ${CMAKE_CURRENT_SOURCE_DIR}/pnglinkage.sip
                   COMMENT "Generating Python Bindings for pnglinkage"
                   DEPENDS pnglinkage.sip VERBATIM)

# Copy __init__.py for the pnglinkage module to its root folder
ADD_CUSTOM_TARGET(copyPngLinkageInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${PYTHON_MODULES_FOLDER}/pnglinkage/__init__.py)
ADD_DEPENDENCIES(wheel copyPngLinkageInitPy)
# Force to use SIP and headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

# Compile the _pnglinkage extension module
ADD_LIBRARY(pypnglinkage SHARED ${PYTHON_BINDINGS_SRC})

# Set rpath of _pnglinkage to $ORIGIN in order for the libpnglinkage to be located
SET_TARGET_PROPERTIES(pypnglinkage PROPERTIES INSTALL_RPATH "$ORIGIN")

# Output the compiled extension module to the pnglinkage module folder once it has been built
SET_TARGET_PROPERTIES(pypnglinkage PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULES_FOLDER}/pnglinkage)

# Ensure sip has been built before compiling the extension module
ADD_DEPENDENCIES(pypnglinkage ${SIP_LIB})

# Set the extension module name to _pnglinkage
SET_TARGET_PROPERTIES(pypnglinkage PROPERTIES OUTPUT_NAME pnglinkage PREFIX "_")

# Link the extension module to libpnglinkage
TARGET_LINK_LIBRARIES(pypnglinkage pnglinkage)

ADD_DEPENDENCIES(wheel pypnglinkage)
