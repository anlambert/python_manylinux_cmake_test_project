CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

SET(PYTHON_MODULES_FOLDER ${CMAKE_BINARY_DIR}/modules)

FIND_PACKAGE(ZLIB REQUIRED)

FIND_PACKAGE(PNG REQUIRED)

FIND_PACKAGE(PythonInterp REQUIRED)

MACRO(COPY_TARGET_LIBRARY_POST_BUILD target_name destination)
  STRING(MD5 DESTINATION_HASH "${destination}")
  SET(COPY_TARGET_NAME copy-${target_name}-${DESTINATION_HASH})

  ADD_CUSTOM_TARGET(${COPY_TARGET_NAME} ALL
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_SONAME_FILE:${target_name}> ${destination}/$<TARGET_SONAME_FILE_NAME:${target_name}>
                  DEPENDS ${target_name}
                  VERBATIM)

  # optionnal parameters of the macro corresponds to targets that depend on the above created custom target
  SET(DEPENDENCIES_TARGETS ${ARGN})

  FOREACH(DEPENDENCY_TARGET ${DEPENDENCIES_TARGETS})
    ADD_DEPENDENCIES(${DEPENDENCY_TARGET} ${COPY_TARGET_NAME})
  ENDFOREACH()
ENDMACRO(COPY_TARGET_LIBRARY_POST_BUILD)

IF(PYTHONINTERP_FOUND)

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

  FIND_PACKAGE(PythonLibs REQUIRED)

  IF(PYTHONLIBS_FOUND)

    SET(SIP_LIB mysip)
    SET(SIP_VERSION_THIRDPARTY 4.18)
    # Build sip code generator 
    TRY_COMPILE(SIP_OK ${CMAKE_CURRENT_BINARY_DIR}/sip-${SIP_VERSION_THIRDPARTY}/sipgen ${CMAKE_CURRENT_SOURCE_DIR}/sip-${SIP_VERSION_THIRDPARTY}/sipgen sip
                CMAKE_FLAGS -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES} -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
                            -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} -DPYTHON_INCLUDE_DIR2=${PYTHON_INCLUDE_DIR2}
                            -DPYTHON_INCLUDE_PATH=${PYTHON_INCLUDE_PATH} -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
                            -DSIP_LIB=${SIP_LIB})
    IF(SIP_OK)
      SET(SIP_VERSION_STR "${SIP_VERSION_THIRDPARTY}")
      SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/sip-${SIP_VERSION_THIRDPARTY}/sipgen/sip)
      SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sip-${SIP_VERSION_THIRDPARTY}/siplib/)

      EXECUTE_PROCESS(COMMAND ${PYTHON_HOME_PATH}/pip install --upgrade wheel)

      # When building Python binary wheels on Linux, produced binaries have to be patched
      # in order for the tulip modules to be successfully imported and loaded on every computer.
      # The 'auditwheel' tool (see https://github.com/pypa/auditwheel) has been developed
      # in order to ease that patching task, so install its latest version with pip first.
      # The tool is only compatible with Python 3.X, so we will use Python 3.5 installed
      # in the docker image provided by the manylinux project (see https://github.com/pypa/manylinux).
      EXECUTE_PROCESS(COMMAND /opt/python/cp35-cp35m/bin/pip install --upgrade auditwheel)

      ADD_CUSTOM_TARGET(wheel
                        COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
                        COMMAND /opt/python/cp35-cp35m/bin/auditwheel repair -L native -w ./dist ./dist/`ls -t ./dist/ | head -1`
                        WORKING_DIRECTORY ${PYTHON_MODULES_FOLDER})

      ADD_SUBDIRECTORY(sip-${SIP_VERSION_THIRDPARTY})
      ADD_SUBDIRECTORY(zliblinkage)
      ADD_SUBDIRECTORY(pnglinkage)

  ENDIF(SIP_OK)

  ENDIF(PYTHONLIBS_FOUND)
ENDIF(PYTHONINTERP_FOUND)