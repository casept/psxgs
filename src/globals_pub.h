#ifndef PSXGS_GLOBALS_PUB_H
#define PSXGS_GLOBALS_PUB_H
#include <psxgpu.h>
#include <psxgte.h>

#include "types_pub.h"

// The draw env for the back buffer
extern DRAWENV GsDRAWENV;
// The disp env for the back buffer
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
// Last element currently used in the GPU packet storage area
extern PACKET* GsOUT_PACKET_CURSOR;

#endif
