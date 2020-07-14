ALL: BUILD

DEST=/usr/local/share/meabench

QMAKE=qmake

BUILD:
	mkdir -p build
	( cd build; $(SELECTQT) $(QMAKE) ../src/meabench.pro )
	+make -C build

clean:
	rm -rf build

install:;
	install -d $(DEST)/bin
	install -d $(DEST)/lib
	install -d $(DEST)/perl
	install -s build/bin/* $(DEST)/bin/
	install -s build/lib/libmeabase.so.*.*.* $(DEST)/lib/
	install -s build/lib/libmeagui.so.*.*.* $(DEST)/lib/
	install `find perl -type f -and -not -name '*.pm' -and -not -name '*~'` $(DEST)/perl/
	install perl/*.pm $(DEST)/perl/
	buildscripts/makemea $(DEST) /usr/local/bin
	ldconfig $(DEST)/lib/

.PHONY: install BUILD PREP
