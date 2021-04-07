#ifndef PSXGS_FLAGS_PUB_H
#define PSXGS_FLAGS_PUB_H

/* GsInitGraph() flags (Sony) */

// Do not interlace
#define GsNONINTER 0x0
// Interlace
#define GsINTER 0x1
// Offset double buffer from GTE
#define GsOFSGTE 0x0
// Offset double buffer from GPU
#define GsOFSGPU 0x2
// Initialize GPU with ResetGraph(0)
#define GsRESET0 0x0
// Initialize GPU with ResetGraph(3)
#define GsRESET3 0x18

/* GsInitGraph() flags (additional) */

// Disable dithering
#define GsNONDITHER 0
// Enable dithering
#define GsDITHER 1
// Use 16-bit VRAM color depth
#define GsVRAM16BIT 0
// Use 24-bit VRAM color depth
#define GsVRAM24BIT 1

#endif