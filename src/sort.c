/* Implementations for sorting objects that are "loose" (i.e. don't belong in file-format specific files) */

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
    setXY0(fill, (unsigned short)GsDRAWENV.clip.x, (unsigned short)GsDRAWENV.clip.y);
    setWH(fill, (unsigned short)GsDRAWENV.clip.w, (unsigned short)GsDRAWENV.clip.h);
    setRGB0(fill, r, g, b);
    // We want to sort this at the very bottom, so that other primitives can paint over it
    AddPrim((unsigned int*)&otp->tag, fill);
}

void GsSortBoxFill(GsBOXF* bp, GsOT* ot, unsigned short pri) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    TILE* tile = (TILE*)GsGetWorkBase();
#pragma GCC diagnostic pop
    setTile(tile);
    setXY0(tile, (short)bp->x, (short)bp->y);
    setWH(tile, (short)bp->w, (short)bp->h);
    setRGB0(tile, bp->r, bp->g, bp->b);
    AddPrim((unsigned int*)&ot->org[pri], tile);
}