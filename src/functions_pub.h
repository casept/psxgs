#ifndef PSXGS_FUNCTIONS_PUB_H
#define PSXGS_FUNCTIONS_PUB_H

// IMPLEMENTATION DETAIL. DO NOT IMPORT.

#include "types_pub.h"

// Initialize the graphics system. Call this before anything else
void GsInitGraph(unsigned short x_res, unsigned short y_res, unsigned short int1, unsigned short dither,
                 unsigned short vram);

// Initialize 3D-specific parts of the graphics system
void GsInit3D(void);

// Set the address of the area reserved for GPU packet buffering
void GsSetWorkBase(PACKET *base_addr);

// Return pointer to next free space in GPU packet buffer
PACKET *GsGetWorkBase(void);

// Set the addresses of both buffers in VRAM
void GsDefDispBuff(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);

// Initialize the given ordering table
void GsClearOT(unsigned short offset, unsigned short point, GsOT *otp);

// Swap the front and back buffers
void GsSwapDispBuffer(void);

// Get whether the 0th or 1st buffer is the current back buffer
int GsGetActiveBuffer(void);

// Convert the pointers in the given TMD from relative to absolute
void GsMapModelingData(unsigned long *p);
// Link the TMD object with index n to the obj_base handle
void GsLinkObject4(unsigned long tmd, GsDOBJ2 *obj_base, int n);
// Sort the GsDOBJ2 handle's object into the given OT
void GsSortObject4(GsDOBJ2 *objp, GsOT *otp, int shift, unsigned long *scratch);

// Process the GPU commands registered in the given ordering table. Runs asynchronously.
void GsDrawOT(GsOT *ot);

// Add a command to the OT telling the GPU to clear the screen to the given color
void GsSortClear(unsigned char r, unsigned char g, unsigned char b, GsOT *otp);
// Add a command to the OT telling the GPU to draw the given box
void GsSortBoxFill(GsBOXF *bp, GsOT *ot, unsigned short pri);

#endif
