A sample CMake project building and packaging Python extension modules
#######################################################################

That project generates a Python binary wheel through CMake.
Its purpose is to add a test case for a pull request for the auditwheel
project (https://github.com/pypa/auditwheel/pull/28), enabling to
repair Linux binary wheels generated trough the help of the manylinux project (https://github.com/pypa/manylinux).

The wheel produced with this sample CMake project contains two modules : **zliblinkage** and **pnglinkage**.

The **zliblinkage** module contains one extension module *_zliblinkage* linked to
a shared library named *libzliblinkage* itself linked against zlib.

The **pnglinkage** module contains one extension module *_pnglinkage* linked to
a shared library named *libpnglinkage* itself linked against libpng and zlib.

The *libzliblinkage.so* and *libpnglinkage.so* files are already bundled in the wheel
along with the extension modules. Rpaths have been correctly set in order for
the auditwheel tool to no graft those libraries but only the ones provided by the system.
