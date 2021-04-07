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

    // TODO: Honor GTE/GPU offset

    // Prepare envs
    // TODO: Does this conflict with GsDefDispBuff
    SetDefDispEnv(&GsDISPENV, 0, 0, x_res, y_res);
    SetDefDrawEnv(&GsDRAWENV, 0, y_res, x_res, y_res);

    // TODO: Init GsIDMATRIX and GsIDMATRIX2
}

void GsInit3D(void) {
    // Initialize GTE
    InitGeom();
    // Center the GTE to middle of screen
    gte_SetGeomOffset(GsDISPENV.screen.w / 2, GsDISPENV.screen.h / 2);
    // Screen depth (FOV)
    gte_SetGeomScreen(GsDISPENV.screen.w / 2);
    // Set ambient light color to white
    gte_SetBackColor(63, 63, 63);
}

void GsSetWorkBase(PACKET *base_addr) { GsOUT_PACKET_P = base_addr; }

PACKET *GsGetWorkBase(void) {
    // TODO: Waste less RAM
    GsOUT_PACKET_CURSOR += GsMAX_GPU_PACKET_SIZE;
    return GsOUT_PACKET_CURSOR;
}