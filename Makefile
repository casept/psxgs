SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

.PHONY: clean

include PSn00bSDK/template/psn00bsdk-setup.mk

TARGET_AR := lib/libpsxgs.a
.DEFAULT_GOAL := $(TARGET_AR)

# We want to trigger a rebuild on changes to the Makefile to prevent accidental Frankenstein builds.
MAKEFILE = Makefile

# Searches for C and S (assembler) files in local directory
CFILES		= $(notdir $(wildcard src/*.c))
AFILES		= $(notdir $(wildcard src/*.s))

# Determine object files
OFILES		= $(addprefix build/,$(CFILES:.c=.o)) \
			$(addprefix build/,$(AFILES:.s=.o))

INCLUDE	 	+= -isystem psn00bsdk/libpsn00b/include

# C compiler warnings
WARNINGS 	+= -Wall -Wextra -Wpedantic -Werror -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wunreachable-code -Wwrite-strings -fanalyzer
# Flags for debugging
DBGFLAGS	+= -g -O0 -DDEBUG
# Flags for optimized release builds
RELFLAGS	+= -O3 -flto
CFLAGS		+= $(WARNINGS) $(DBGFLAGS) -march=r3000 -mtune=r3000 -fdata-sections -ffunction-sections -fno-builtin -nostdlib --std=c11

# Assembler flags
AFLAGS		+= -g -msoft-float

$(TARGET_AR): $(OFILES)
	@mkdir -p $(dir $@)
	ar -rc $(TARGET_AR) $(OFILES)

build/%.o: src/%.c $(MAKEFILE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

build/%.o: src/%.s $(MAKEFILE)
	@mkdir -p $(dir $@)
	$(CC) $(AFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf build $(TARGET_AR)
