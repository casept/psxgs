#include <psxgpu.h>

#include "functions_pub.h"
#include "globals_pub.h"
#include "types_pub.h"

void GsSortClear(unsigned char r, unsigned char g, unsigned char b, GsOT* otp) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    FILL* fill = (FILL*)GsGetWorkBase();
#pragma GCC diagnostic pop
    setFill(fill);
    setXY0(fill, 0, 0);
    setWH(fill, (unsigned short)GsDISPENV.screen.w, (unsigned short)GsDISPENV.screen.h);
    setRGB0(fill, r, g, b);
    AddPrim((unsigned int*)otp->org, fill);
}

void GsDefDispBuff(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1) {
    PSDBASEX[0] = (short)x0;
    PSDBASEY[0] = (short)y0;
    PSDBASEX[1] = (short)x1;
    PSDBASEY[1] = (short)y1;
    // TODO: Set disp/draw env
}

void GsSwapDispBuffer(void) {
    // Sets display starting address in VRAM
    // TODO: Cancel blanking

    // Set double buffer index
    if (PSDIDX == 0) {
        PSDIDX = 1;
    } else {
        PSDIDX = 0;
    }
    // Switch two-dimensional clipping
    // Set libgte or libgpu offset
    // Set libgs offset
}

int GsGetActiveBuffer(void) { return (int)PSDIDX; }