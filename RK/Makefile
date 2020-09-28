# makefile clean rule idea found here:
# https://stackoverflow.com/questions/33581943/how-to-write-a-makefile-rule-to-remove-files-clean-only-if-the-files-are-prese
# fantastic makefiles tutorial here:
# https://ocw.cs.pub.ro/courses/uso/laboratoare/laborator-03

.phony: all

ASM = nasm
ASMFLAGS = -g -f elf32
LINKASMFLAGS = -g -m32
CC = gcc
CFLAGS = -std=gnu89 -pedantic-errors -Wall -Werror -g -m32 -c
LINKCFLAGS = -g -m32
MATHLINKFLAGS = -lm
CASMFLAGS = -m32 -masm=intel -g -S
DUMPER = objdump
DUMPFLAGS = -M intel

filenames := asm/rk.o asm/rk c/rk c/rk.o ../util/c/utility.o

files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

all: c/rk asm/rk

../util/c/utility.o: ../util/c/utility.c
	$(CC) $(CFLAGS) $(OLEVEL) $^ -o $@

c/rk: c/rk.o ../util/c/utility.o
	$(CC) $(LINKCFLAGS) $^ -o $@ $(MATHLINKFLAGS)

c/rk.o: c/rk.c ../util/c/utility.h
	$(CC) $(CFLAGS) $(OLEVEL) $< -o $@

asm/rk: asm/rk.o
	$(CC) $(LINKASMFLAGS) $^ -o $@

asm/rk.o: asm/rk.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

clean:
ifneq ($(files),)
	rm -f $(files)
endif

cleancasm: c/rk.asm
	rm -rf $^

runasm: asm/rk
	$^

runc: c/rk
	@$^ $(ARGS)

runcverbose: c/rk
	@$^ --verbose $(ARGS)

c/rk.asm: c/rk.c
	$(CC) $(CASMFLAGS) $^ -o $@

dumpc: c/rk.o
	$(DUMPER) $(DUMPFLAGS) -d $^

dumpasm: asm/rk.o
	$(DUMPER) $(DUMPFLAGS) -d $^

check: c/ac
	cd c; python checker.py; cd ..
