TARGET = mysip.so
OFILES = siplib.o apiversions.o descriptors.o qtlib.o threads.o objmap.o voidptr.o array.o bool.o
HFILES = sip.h sipint.h array.h 

CC = gcc
CXX = g++
LINK = g++
CPPFLAGS = -DNDEBUG -I. -I/usr/include/python2.7
CFLAGS = -pipe -fPIC -O2 -Wall -W
CXXFLAGS = -pipe -fPIC -O2 -Wall -W
LFLAGS = -shared -Wl,--version-script=mysip.exp
LIBS = 
.SUFFIXES: .c .o .cpp .cc .cxx .C


.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

all: $(TARGET)

$(OFILES): $(HFILES)

$(TARGET): $(OFILES)
	@echo '{ global: initmysip; local: *; };' > mysip.exp
	$(LINK) $(LFLAGS) -o $(TARGET) $(OFILES) $(LIBS)

install: $(TARGET)
	@test -d $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib || mkdir -p $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib
	cp -f $(TARGET) $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib/$(TARGET)
	strip $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib/$(TARGET)
	@test -d $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/sip-4.18/siplib || mkdir -p $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/sip-4.18/siplib
	cp -f /home/antoine/dev/python_manylinux_test_project/sip-4.18/siplib/sip.h $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/sip-4.18/siplib/sip.h
	@test -d $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib || mkdir -p $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib
	cp -f /home/antoine/dev/python_manylinux_test_project/sip-4.18/sip.pyi $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib/sip.pyi

clean:
	-rm -f $(TARGET)
	-rm -f siplib.o
	-rm -f apiversions.o
	-rm -f descriptors.o
	-rm -f qtlib.o
	-rm -f threads.o
	-rm -f objmap.o
	-rm -f voidptr.o
	-rm -f array.o
	-rm -f bool.o
	-rm -f mysip.exp
