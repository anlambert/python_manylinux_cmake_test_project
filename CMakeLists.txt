# Sample CMake project building a Python wheel containing two modules : zliblinkage and pnglinkage.
# Each module contains a C extension modules generated trough the help of the SIP tool (https://www.riverbankcomputing.com/software/sip):
#  - _zliblinkage for zliblinkage : linked to zlib
#  - _pnglinkage for pnglinkage: linked to libpng but also zlib as it is a libpng dependency
# Each extension module is linked to a shared library built in this project:
#  - libzliblinkage for _zliblinkage
#  - libpnglinkage for _pnglinkage
# When building the project, all files that need to be bundled in the wheel are copied in the PYTHON_MODULES_FOLDER directory.
# The binary wheel is then created using small tricks for distutils and setuptools (see setup.py and setup.cfg)
# in order to get the correct wheel tag name (e.g. cp35-cp35m-linux_x86_64)

# To build this project, proceed the following way:
# $ cmake -DPYTHON_EXECUTABLE=<path_to_python> <source_dir>
# $ make
# $ make wheel
# The produced wheel will be located in <build_dir>/modules/dist

# CMake 2.8 is required to build the project
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

FILE(READ /etc/issue LINUX_ISSUE)
IF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT IS_DIRECTORY /opt/python)
 MESSAGE(FATAL_ERROR "This project must be compiled using the CentOS 5.11 docker image provided by the manylinux project from the Python Packaging Authority.\nSee https://github.com/pypa/manylinux for more details.")
ENDIF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT IS_DIRECTORY /opt/python)

# variable that hold the root modules directory
SET(PYTHON_MODULES_FOLDER ${CMAKE_BINARY_DIR}/modules)

# find zlib
FIND_PACKAGE(ZLIB REQUIRED)

# find libpng
FIND_PACKAGE(PNG REQUIRED)

# find Python
FIND_PACKAGE(PythonInterp REQUIRED)

# Tell CMake to build libraries with install rpath embedded in them
# as we don't install the project and we set rpath to extension modules
# in order for them to locate their shared library dependencies
SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

IF(PYTHONINTERP_FOUND)

  # Get Python version
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} --version OUTPUT_VARIABLE PYTHON_VERSION_RAW ERROR_VARIABLE PYTHON_VERSION_RAW)
  STRING(REPLACE "\n" "" PYTHON_VERSION_RAW "${PYTHON_VERSION_RAW}")
  STRING(REGEX MATCH "[0-9]\\.[0-9]" PYTHON_VERSION "${PYTHON_VERSION_RAW}")
  STRING(REGEX MATCH "[0-9]\\.[0-9]\\.[0-9]" PYTHON_VERSION_WITH_PATCH "${PYTHON_VERSION_RAW}")

  # Unset the previous values of the CMake cache variables related to Python libraries
  # in case the value of PYTHON_EXECUTABLE CMake variable changed
  UNSET(PYTHONLIBS_FOUND CACHE)
  UNSET(PYTHON_LIBRARY CACHE)
  UNSET(PYTHON_INCLUDE_DIR CACHE)
  UNSET(PYTHON_INCLUDE_PATH CACHE)

  # Find the Python library with the same version as the interpreter
  # Python 3.2 library is suffixed by mu and Python >= 3.3 by m on some systems, also handle these cases
  SET(Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION}mu ${PYTHON_VERSION}m ${PYTHON_VERSION})

  GET_FILENAME_COMPONENT(PYTHON_HOME_PATH ${PYTHON_EXECUTABLE} PATH)

  # Ensure that correct Python include path and library are selected by CMake on Linux (in case of non standard installation)
  SET(CMAKE_INCLUDE_PATH ${PYTHON_HOME_PATH}/../include ${CMAKE_INCLUDE_PATH})
  SET(CMAKE_LIBRARY_PATH ${PYTHON_HOME_PATH}/../lib ${CMAKE_LIBRARY_PATH})

  # Find Python includes and library
  FIND_PACKAGE(PythonLibs REQUIRED)

  IF(PYTHONLIBS_FOUND)

    # Build SIP code generator in order to produce the source code of the extension modules of the project
    SET(SIP_LIB sip_manylinux_tests)
    SET(SIP_VERSION_THIRDPARTY 4.18)
    TRY_COMPILE(SIP_OK ${CMAKE_CURRENT_BINARY_DIR}/sip-${SIP_VERSION_THIRDPARTY}/sipgen ${CMAKE_CURRENT_SOURCE_DIR}/sip-${SIP_VERSION_THIRDPARTY}/sipgen sip
                CMAKE_FLAGS -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES} -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
                            -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} -DPYTHON_INCLUDE_DIR2=${PYTHON_INCLUDE_DIR2}
                            -DPYTHON_INCLUDE_PATH=${PYTHON_INCLUDE_PATH} -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
                            -DSIP_LIB=${SIP_LIB})
    IF(SIP_OK)
      SET(SIP_VERSION_STR "${SIP_VERSION_THIRDPARTY}")
      SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/sip-${SIP_VERSION_THIRDPARTY}/sipgen/sip)
      SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sip-${SIP_VERSION_THIRDPARTY}/siplib/)

      # Ensure the wheel module is installed
      EXECUTE_PROCESS(COMMAND ${PYTHON_HOME_PATH}/pip install --upgrade wheel)

      # Add a make target for generating the wheel
      ADD_CUSTOM_TARGET(wheel
                        COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
                        WORKING_DIRECTORY ${PYTHON_MODULES_FOLDER})

      # Compile the sip module required by the extension modules
      ADD_SUBDIRECTORY(sip-${SIP_VERSION_THIRDPARTY})
      # Compile the extension modules of the project
      ADD_SUBDIRECTORY(zliblinkage)
      ADD_SUBDIRECTORY(pnglinkage)

      # Copy packaging files to the modules root folder
      ADD_CUSTOM_TARGET(copyPackagingFiles ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg ${PYTHON_MODULES_FOLDER}/setup.cfg
                                           COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in ${PYTHON_MODULES_FOLDER}/MANIFEST.in
                                           COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/setup.py ${PYTHON_MODULES_FOLDER}/setup.py)

  ENDIF(SIP_OK)

  ENDIF(PYTHONLIBS_FOUND)
ENDIF(PYTHONINTERP_FOUND)
