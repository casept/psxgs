SHELL           := bash
.SHELLFLAGS		:= -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS		+= --warn-undefined-variables
MAKEFLAGS		+= --no-builtin-rules

.PHONY: clean run

# Where you have installed PSn00bSDK. This path is correct if you've checked it out as a submodule in the repo root.
PSN00B_ROOT		= ../../PSn00bSDK
# Where you have installed the psxgs library and headers to. The default is valid if they're installed in the repo root.
PSXGS_ROOT		= ../..

include $(PSN00B_ROOT)/template/psn00bsdk-setup.mk

TARGET_EXE		= triangle.exe
TARGET_ELF		= triangle.elf
.DEFAULT_GOAL	:= $(TARGET_EXE)

# Some emulators require an ISO to be specified to load an ELF.
# All data is compiled into the binary, this image serves no other purpose.
DUMMY_ISO	= dummy.iso
# We want to trigger a rebuild on changes to the Makefile to prevent accidental Frankenstein builds.
MAKEFILE = Makefile

# Searches for C and asm files in local directory.
CFILES		= $(addprefix src/,$(notdir $(wildcard src/*.c)))
AFILES		= $(addprefix src/,$(notdir $(wildcard src/*.s)))

# Determine object files.
OFILES		= $(addprefix build/,$(CFILES:.c=.o)) \
	$(addprefix build/,$(AFILES:.s=.o))


# Include path.
INCLUDE	 	+= -isystem $(PSN00B_ROOT)/libpsn00b/include -isystem $(PSXGS_ROOT)/include
# Libraries.
LIBDIRS     += -L$(PSN00B_ROOT)/libpsn00b -L$(PSXGS_ROOT)/lib/
LIBS        += -lpsxgs -lpsxgpu -lpsxgte -lpsxetc -lpsxapi -lpsxsio -lc

# Compiler flags.
WARNINGS 	+= -Wall -Wextra -Wpedantic -Werror -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wunreachable-code -Wwrite-strings
# Flags for debugging.
DBGFLAGS	+= -g -O0 -DDEBUG
# Flags for optimized release builds.
RELFLAGS	+= -O3 -flto
# Replace with RELFLAGS and run `make clean` to switch between release and debug builds.
CFLAGS		+= $(WARNINGS) $(DBGFLAGS) -march=r3000 -mtune=r3000 -fdata-sections -ffunction-sections -fno-builtin -nostdlib --std=c11

# Assembler flags.
AFLAGS		+= -g -msoft-float

# Linker flags.
LDFLAGS		+= -g -flto -Ttext=0x80010000 -gc-sections -T ../linker.ld

build/%.o: %.c $(MAKEFILE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

build/%.o: %.s $(MAKEFILE)
	@mkdir -p $(dir $@)
	$(CC) $(AFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET_ELF): $(OFILES)
	$(LD) $(LDFLAGS) $(LIBDIRS) $(OFILES) $(LIBS) -o $(TARGET_ELF)

$(TARGET_EXE): $(TARGET_ELF)
	elf2x -q $(TARGET_ELF) $(TARGET_EXE)

# This is just a convenience target. Replace with whathever invocation is appropriate for your emulator.
run: $(TARGET_ELF) $(DUMMY_ISO)
	pcsxr -gdb -psxout -nogui -cdfile $(DUMMY_ISO) $(TARGET_ELF)

clean:
	rm -rf build $(TARGET_ELF) $(TARGET_EXE)
