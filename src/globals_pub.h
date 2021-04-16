#ifndef PSXGS_GLOBALS_PUB_H
#define PSXGS_GLOBALS_PUB_H
#include <psxgpu.h>
#include <psxgte.h>

#include "types_pub.h"

// The libgte draw environment
extern DRAWENV GsDRAWENV;
// The libgte display environment
extern DISPENV GsDISPENV;

// The X coordinate of the double buffers
extern short PSDBASEX[2];
// The Y coordinate of the double buffers
extern short PSDBASEY[2];
// Which of the buffers is the back buffer right now
extern short PSDIDX;
// Frame counter
extern unsigned long PSDCNT;

// Unit matrix
extern MATRIX GsIDMATRIX;
// Unit matrix with aspect conversion
extern MATRIX GsIDMATRIX2;

// Start of the GPU packet storage area
extern PACKET* GsOUT_PACKET_P;
// First currently free space in the GPU packet storage area
extern PACKET* GsOUT_PACKET_CURSOR;

// The top-level coordinate system. All other coordinate systems must have this as their indirect superior.
extern GsCOORDINATE2 WORLD;
// World-to-screen matrix used by default
extern MATRIX GsWSMATRIX;

#endif
