# great idea for this makefile provided here:
# https://stackoverflow.com/questions/17834582/run-make-in-each-subdirectory

TOPTARGETS := all clean

SUBDIRS := $(wildcard */.)

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)

check: KMP/c/kmp RK/c/rk AC/c/ac
	cd KMP; make -s clean; make -s OLEVEL=-O3; cd c; python checker.py; cd ../..; cd RK; make -s clean; make -s OLEVEL=-O3; cd c; python checker.py; cd ../..; cd AC; make -s clean; make -s OLEVEL=-O3; cd c; python checker.py; cd ../..
