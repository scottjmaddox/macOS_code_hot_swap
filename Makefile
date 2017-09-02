MAIN=build/main
HOT_MAIN=build/hot_main.dylib

MAIN_C_DEPS=src/main.c
MAIN_H_DEPS=src/hot_main.h

HOT_MAIN_C_DEPS=src/hot_main.c
HOT_MAIN_H_DEPS=src/hot_main.h

# Program for compiling C programs; default `cc`
CC=cc

# Extra flags to give to the C compiler.
CFLAGS=-g -std=c99 -Wall -Wextra -Wno-unused-function -Wno-unused-variable

# Extra flags to give to compilers when they are supposed to invoke the linker,
# ‘ld’, such as -L. Libraries (-lfoo) should be added to the LDLIBS variable
# instead.
LDFLAGS=

# Library flags or names given to compilers when they are supposed to invoke
# the linker, ‘ld’. LOADLIBES is a deprecated (but still supported) alternative
# to LDLIBS. Non-library linker flags, such as -L, should go in the LDFLAGS
# variable.
LDLIBS=

all: build $(HOT_MAIN) $(MAIN)

build:
	mkdir -p build

$(HOT_MAIN): $(HOT_MAIN_C_DEPS) $(HOT_MAIN_H_DEPS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) \
		-dynamiclib -current_version 1.0 -compatibility_version 1.0 \
		-o $@ \
		$(HOT_MAIN_C_DEPS)

$(MAIN): $(MAIN_C_DEPS) $(MAIN_H_DEPS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) \
		-o $@ \
		$(MAIN_C_DEPS)

clean:
	rm -rf build
