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
CASMFLAGS = -m32 -masm=intel -g -S
DUMPER = objdump
DUMPFLAGS = -M intel

filenames := asm/kmp.o asm/kmp c/kmp c/kmp.o c/utility.o ../util/c/utility.o

files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

all: c/kmp asm/kmp

../util/c/utility.o: ../util/c/utility.c
	$(CC) $(CFLAGS) $(OLEVEL) $^ -o $@

c/kmp: c/kmp.o ../util/c/utility.o
	$(CC) $(LINKCFLAGS) $^ -o $@

c/kmp.o: c/kmp.c ../util/c/utility.h
	$(CC) $(CFLAGS) $(OLEVEL) $< -o $@

asm/kmp: asm/kmp.o
	$(CC) $(LINKASMFLAGS) $^ -o $@

asm/kmp.o: asm/kmp.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

clean: 
ifneq ($(files),)
	rm -f $(files)
endif

cleancasm: c/kmp.asm
	rm -rf $^

runasm: asm/kmp
	$^

runc: c/kmp
	@$^ $(ARGS)

runcverbose: c/kmp
	@$^ --verbose $(ARGS)

c/kmp.asm: c/kmp.c
	$(CC) $(CASMFLAGS) $^ -o $@

dumpc: c/kmp.o
	$(DUMPER) $(DUMPFLAGS) -d $^

dumpasm: asm/kmp.o
	$(DUMPER) $(DUMPFLAGS) -d $^ 

preprocess: c/kmp.c
	$(CC) -DINFOARENA -E $^ > preprocess.c

check: c/ac
	cd c; python checker.py; cd ..
