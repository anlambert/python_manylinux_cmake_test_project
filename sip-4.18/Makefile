
all:
	@(cd sipgen; $(MAKE))
	@(cd siplib; $(MAKE))

install:
	@(cd sipgen; $(MAKE) install)
	@(cd siplib; $(MAKE) install)
	@test -d $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib || mkdir -p $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib
	cp -f sipconfig.py $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib/sipconfig.py
	cp -f /home/antoine/dev/python_manylinux_test_project/sip-4.18/sipdistutils.py $(DESTDIR)/home/antoine/dev/python_manylinux_test_project/build/sip-4.18/siplib/sipdistutils.py

clean:
	@(cd sipgen; $(MAKE) clean)
	@(cd siplib; $(MAKE) clean)
