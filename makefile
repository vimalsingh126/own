CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -O2
LDFLAGS = -L/path/to/libssl -L/path/to/libcrypto -L/path/to/libssh -L/path/to/ltss2-sys -L/path/to/ltss2-tctildr
LIBS = -lssl -lcrypto -lssh -ltss2-sys -ltss2-tctildr

all: program

program: program.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

program.o: program.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f program program.o







####################################

CC=gcc
CFLAGS=-Wall -O2
LDFLAGS=-L/path/to/libraries

SRCDIR=src
BINDIR=bin

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.o,$(SOURCES))
EXECUTABLE=$(BINDIR)/my_program

LIBS=-lssl -lcrypto -lssh -ltss2-sys -ltss2-tctildr

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	
	
	
	    AddressSanitizer (ASan): a tool built into the Clang and GCC compilers that detects memory errors at runtime, including use-after-free, buffer overflow, and memory leak.

    Electric Fence (EFence): a library that can be linked with C programs to detect memory errors such as buffer overflows, out-of-bounds reads and writes, and use-after-free.

    Purify: a commercial tool that can detect memory leaks, buffer overflows, and other memory-related errors in C and C++ programs.

    Memcheck: a tool included in the Valgrind suite that detects memory leaks, uninitialized memory reads, and other memory errors.

    LeakSanitizer (LSan): a tool built into the Clang and GCC compilers that detects memory leaks at runtime, similar to ASan.









sudo valgrind --leak-check=full --show-leak-kinds=all --log-file=memcheck.log ./nvreadsudo valgrind --leak-check=full --show-leak-kinds=all --log-file=memcheck.log ./nvread

sudo valgrind --leak-check=full --log-file=memcheck.log ./nvread
