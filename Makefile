# Compiler
CC = gcc
#CC = clang

# location of headers
INCLUDE = -I./src/internal

# list of all C files
SRCS = src/main.c $(wildcard src/internal/*.c)

# Debug build objects
OBJS_DEBUG = $(patsubst src/%.c, build/debug/%.o, $(SRCS))

# create 
OBJS_RELEASE = $(patsubst src/%.c, build/release/%.o, $(SRCS))

# Targets
TARGET = mefetch_debug
RELEASE_TARGET = mefetch_release

# use strict mode and also use fsanitize and g so memory leaks are reported and valgrind can be used
CFLAGS_DEBUG = -Wall -Wextra -Werror -fsanitize=address -g $(INCLUDE)
# optimize code 
CFLAGS_RELEASE = -Wall -Wextra -O3 $(INCLUDE)
# sqlite dev lib is needed
LDFLAGS = 
#LDFLAGS = -lsqlite3

# default rule: debug build
all: $(TARGET)

# debug build
$(TARGET): $(OBJS_DEBUG)
	$(CC) $^ -o $@ $(LDFLAGS) $(CFLAGS_DEBUG)

build/debug/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS_DEBUG)

# release build
release: $(OBJS_RELEASE)
	$(CC) $^ -o $(RELEASE_TARGET) $(LDFLAGS)  $(CFLAGS_RELEASE)


build/release/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS_RELEASE)

clean:
	rm -rf build $(TARGET) $(RELEASE_TARGET)

.PHONY: all clean release
