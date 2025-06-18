include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard c/[^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
INCLUDE_DIRS=-I./include

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) $(INCLUDE_DIRS) -T sampleCodeModule.ld c/_loader.c $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)

%.o: %.c
	$(GCC) $(GCCFLAGS) $(INCLUDE_DIRS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf c/*.o
	rm -rf asm/*.o

.PHONY: all clean print