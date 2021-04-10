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
    // We want to sort this at the very top
    AddPrim((unsigned int*)otp->tag, fill);
}

void GsDefDispBuff(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1) {
    PSDBASEX[0] = (short)x0;
    PSDBASEY[0] = (short)y0;
    PSDBASEX[1] = (short)x1;
    PSDBASEY[1] = (short)y1;

    SetDefDrawEnv(&GsDRAWENV, PSDBASEX[0], PSDBASEY[0], GsDRAWENV.clip.w, GsDRAWENV.clip.h);
    SetDefDispEnv(&GsDISPENV, PSDBASEX[1], PSDBASEY[1], GsDISPENV.disp.w, GsDISPENV.disp.h);
    PutDispEnv(&GsDISPENV);
    PutDrawEnv(&GsDRAWENV);
}

void GsSwapDispBuffer(void) {
    // Set the DISPENV to display the old back buffer
    SetDefDispEnv(&GsDISPENV, PSDBASEX[PSDIDX], PSDBASEY[PSDIDX], GsDISPENV.disp.w, GsDISPENV.disp.h);
    PutDispEnv(&GsDISPENV);

    // Swap double buffer index
    PSDIDX = !PSDIDX;

    // Set the DRAWENV to draw into the new back buffer
    SetDefDrawEnv(&GsDRAWENV, PSDBASEX[PSDIDX], PSDBASEY[PSDIDX], GsDRAWENV.clip.w, GsDRAWENV.clip.h);
    PutDrawEnv(&GsDRAWENV);

    // TODO: Set libgte or libgpu offset (?)
    // TODO: Set libgs offset (?)

    // Enable display
    SetDispMask(1);
}

int GsGetActiveBuffer(void) { return (int)PSDIDX; }