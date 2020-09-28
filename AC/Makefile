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

filenames := asm/ac.o asm/ac c/ac c/ac.o ../util/c/utility.o ../util/c/queue.o

files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

all: c/ac

../util/c/utility.o: ../util/c/utility.c
	$(CC) $(CFLAGS) $(OLEVEL) $^ -o $@

../util/c/queue.o: ../util/c/queue.c
	$(CC) $(CFLAGS) $(OLEVEL) $^ -o $@

c/ac: c/ac.o ../util/c/queue.o ../util/c/utility.o
	$(CC) $(LINKCFLAGS) $^ -o $@

c/ac.o: c/ac.c ../util/c/utility.h
	$(CC) $(CFLAGS) $(OLEVEL) $< -o $@

asm/ac: asm/ac.o
	$(CC) $(LINKASMFLAGS) $^ -o $@

asm/ac.o: asm/ac.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

clean: 
ifneq ($(files),)
	rm -f $(files)
endif

cleancasm: c/ac.asm
	rm -rf $^

runasm: asm/ac
	$^

runc: c/ac
	@$^ $(ARGS)

runcverbose: c/ac
	@$^ --verbose $(ARGS)

c/ac.asm: c/ac.c
	$(CC) $(CASMFLAGS) $^ -o $@

dumpc: c/ac.o
	$(DUMPER) $(DUMPFLAGS) -d $^

dumpasm: asm/ac.o
	$(DUMPER) $(DUMPFLAGS) -d $^ 

preprocess: c/ac.c
	$(CC) -DINFOARENA -E $^ > preprocess.c

check: c/ac
	cd c; python checker.py; cd ..
