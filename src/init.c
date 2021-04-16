#include <inline_c.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <stdbool.h>

#include "dbg.h"
#include "flags_pub.h"
#include "functions_pub.h"
#include "globals_pub.h"

#define GsMAX_GPU_PACKET_SIZE 24

void GsInitGraph(unsigned short x_res, unsigned short y_res, unsigned short int1, unsigned short dither,
                 unsigned short vram) {
    // Reset graphics
    if ((int1 & GsRESET0) == GsRESET0) {
        ResetGraph(0);
    } else if ((int1 & GsRESET3) == GsRESET3) {
        ResetGraph(3);
    } else {
        GsFatal("GsInitGraph(): Unknown graphics reset flag passed");
    }

    // Prepare envs (but don't apply yet, as the framebuffer areas are defined by calling GsDefDispBuff)
    SetDefDispEnv(&GsDISPENV, 0, 0, x_res, y_res);
    SetDefDrawEnv(&GsDRAWENV, 0, 0, x_res, y_res);

    // Setup interlace
    if ((int1 & GsINTER) == GsINTER) {
        GsDISPENV.isinter = 1;
    } else {
        GsDISPENV.isinter = 0;
    }

    // Setup dithering
    if (dither == GsDITHER) {
        GsDRAWENV.dtd = 1;
    } else {
        GsDRAWENV.dtd = 0;
    }

    // Setup VRAM color depth
    if (vram == GsVRAM24BIT) {
        GsDISPENV.isrgb24 = 1;
    } else {
        GsDISPENV.isrgb24 = 0;
    }

    // TODO: Honor GTE/GPU offset (once I figure out what Sony means by them)

    // TODO: Init GsIDMATRIX and GsIDMATRIX2
}

void GsInit3D(void) {
    // Initialize GTE
    InitGeom();
    // Center the GTE to middle of screen
    gte_SetGeomOffset(GsDISPENV.disp.w / 2, GsDISPENV.disp.h / 2);
    // Set screen depth (FOV)
    gte_SetGeomScreen(GsDISPENV.disp.w / 2);
    // Set ambient light color to white
    gte_SetBackColor(63, 63, 63);
}

void GsSetWorkBase(PACKET *base_addr) {
    GsOUT_PACKET_P = base_addr;
    GsOUT_PACKET_CURSOR = base_addr;
}

PACKET *GsGetWorkBase(void) {
    PACKET *old_cursor = GsOUT_PACKET_CURSOR;
    GsOUT_PACKET_CURSOR += GsMAX_GPU_PACKET_SIZE;
    return old_cursor;
}