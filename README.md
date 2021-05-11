# psxgs

This is a **heavily WIP** re-implementation of Sony's `libgs` Playstation 1 high-level graphics library. It aims to be API-compatible with the official library so that old games and SDK sample code can be ported over.

Currently, it's not even at the point of rendering a TMD triangle yet.

Once reasonably complete, I hope to merge it into [Psn00bSDK](https://github.com/Lameguy64/PSn00bSDK).

## Building

1. Obtain a cross-compiler which supports at least C11. I'm using one built using [crosstool-ng](https://crosstool-ng.github.io/) at commit `bbb0db2880d5e7600e176fc8025f0add9ef85eb9`. Simply follow the instructions on the crosstool-ng site to build and install it, then do `cp mipsel-sony-none.config .config` and `ct-ng build`. Copy the resulting toolchain to `toolchains/`.
2. Follow the Psn00bSDK guide to build and install the SDK, then run `make` in the directory you cloned `psxgs` into.

### Building samples

After building the library, `cd samples && make` will create a PSX `.exe` executable for each sample. Any half-decent emulator should be able to run these directly.

## Usage

Add `/path/to/psxgs/include` to your build system's system headers (for GCC, this can be done using `-isystem`), as well as `/path/to/psxgs/lib/` to your build system's static library search path. Finally, add `-l psxgs` to link against the library. Note that it has to be placed before `-l pxsgpu` and `-l psxgte`, otherwise the linker won't find the symbols `psxgs` needs.

In case of problems, it may be helpful to reference the `Makefiles` under `sample/`.
