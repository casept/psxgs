#include "dbg.h"

#include <psxgpu.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "math.h"
#include "tmd.h"
#include "types_pub.h"

void GsFatal(const char *msg) {
    printf("[psxgs] Fatal psxgs error: %s\n", msg);
    while (true) {
    }
}

void GsAssert(const int e) {
    if (!e) {
        GsFatal("assertion failed");
    }
}

/*
 * Pretty-print various data types
 */

void GsDumpTMDStruct(const GsTMDObject *t) {
    printf("[psxgs] GsTMDObject: {vertop: 0x%X vern: %lu nortop: 0x%X norn: %lu primtop: 0x%X primn: %lu scale: %d}\n",
           t->vertop, t->vern, t->nortop, t->norn, t->primtop, t->primn, t->scale);
}

void GsDumpTMDHeader(const GsTMDHeader *h) {
    printf("[psxgs] GsTMDHeader: {id: 0x%X, flags: 0x%X, nobj: %lu}\n", h->id, h->flags, h->nobj);
}

void GsDumpTMDPF3(const GsTMDPF3 *f3) {
    // TODO: Other fields
    printf("[psxgs] GsTMDPF3: {hdr: {olen: %u, ilen: %u, flag: %u, mode: %u}}\n", f3->hdr.olen, f3->hdr.ilen,
           f3->hdr.flag, f3->hdr.mode);
}

void GsDumpTMDPF4(const GsTMDPF4 *f4) {
    // TODO: Other fields
    printf("[psxgs] GsTMDPF4: {hdr: {olen: %u, ilen: %u, flag: %u, mode: %u}}\n", f4->hdr.olen, f4->hdr.ilen,
           f4->hdr.flag, f4->hdr.mode);
}

void GsDumpTMDPacketHeader(const GsTMDPacketHeader *hdr) {
    printf("[psxgs] GsTMDPacketHeader: {hdr: {olen: %u, ilen: %u, flag: %u, mode: %u}}\n", hdr->olen, hdr->ilen,
           hdr->flag, hdr->mode);
}

typedef enum {
    PRIM_POLY_F3 = 0x20,
    PRIM_POLY_FT3 = 0x24,
    PRIM_POLY_G3 = 0x30,
    PRIM_POLY_GT3 = 0x34,

    PRIM_POLY_F4 = 0x28,
    PRIM_POLY_FT4 = 0x2c,
    PRIM_POLY_G4 = 0x38,
    PRIM_POLY_GT4 = 0x3c,

    PRIM_PRIM_SPRT_8 = 0x74,
    PRIM_PRIM_SPRT_16 = 0x7c,
    PRIM_SPRT = 0x64,

    PRIM_TILE_1 = 0x68,
    PRIM_TILE_8 = 0x70,
    PRIM_TILE_16 = 0x78,
    PRIM_TILE = 0x60,

    PRIM_LINE_F2 = 0x40,
    PRIM_LINE_G2 = 0x50,
    PRIM_LINE_F3 = 0x48,
    PRIM_LINE_G3 = 0x58,
    PRIM_LINE_F4 = 0x4c,
    PRIM_LINE_G4 = 0x5c,

    PRIM_FILL = 0x02
} GsOTPrimCode;

static void GsGetOTPrimName(char *name, GsOTPrimCode prim_code) {
    switch (prim_code) {
        case PRIM_POLY_F3:
            strcpy(name, "PRIM_POLY_F3\0");
            break;
        case PRIM_POLY_FT3:
            strcpy(name, "PRIM_POLY_FT3\0");
            break;
        case PRIM_POLY_G3:
            strcpy(name, "PRIM_POLY_G3\0");
            break;
        case PRIM_POLY_GT3:
            strcpy(name, "PRIM_POLY_GT3\0");
            break;

        case PRIM_POLY_F4:
            strcpy(name, "PRIM_POLY_F4\0");
            break;
        case PRIM_POLY_FT4:
            strcpy(name, "PRIM_POLY_FT4\0");
            break;
        case PRIM_POLY_G4:
            strcpy(name, "PRIM_POLY_G4\0");
            break;
        case PRIM_POLY_GT4:
            strcpy(name, "PRIM_POLY_GT4\0");
            break;

        case PRIM_PRIM_SPRT_8:
            strcpy(name, "PRIM_PRIM_SPRT_8\0");
            break;
        case PRIM_PRIM_SPRT_16:
            strcpy(name, "PRIM_PRIM_SPRT_16\0");
            break;
        case PRIM_SPRT:
            strcpy(name, "PRIM_SPRT\0");
            break;

        case PRIM_TILE_1:
            strcpy(name, "PRIM_TILE_1\0");
            break;
        case PRIM_TILE_8:
            strcpy(name, "PRIM_TILE_8\0");
            break;
        case PRIM_TILE_16:
            strcpy(name, "PRIM_TILE_16\0");
            break;
        case PRIM_TILE:
            strcpy(name, "PRIM_TILE\0");
            break;

        case PRIM_LINE_F2:
            strcpy(name, "PRIM_LINE_F2\0");
            break;
        case PRIM_LINE_G2:
            strcpy(name, "PRIM_LINE_G2\0");
            break;
        case PRIM_LINE_F3:
            strcpy(name, "PRIM_LINE_F3\0");
            break;
        case PRIM_LINE_G3:
            strcpy(name, "PRIM_LINE_G3\0");
            break;
        case PRIM_LINE_F4:
            strcpy(name, "PRIM_LINE_F4\0");
            break;
        case PRIM_LINE_G4:
            strcpy(name, "PRIM_LINE_G4\0");
            break;

        case PRIM_FILL:
            strcpy(name, "PRIM_FILL\0");
            break;
        default:
            sprintf(name, "UNKNOWN(0x%X)\0", (unsigned char)prim_code);
            break;
    }
}

void GsDumpOT(const GsOT *ot) {
    printf("[psxgs] GsOT: {length (AKA precision): %lu, offset: %lu, point: %lu, contents: <Start> ", ot->length,
           ot->offset, ot->point);

    // Walk the OT in the order the GPU does, printing primitives
    GsOT_TAG *prim = ot->tag;
    while (!isendprim(prim)) {
        // We don't care about filler/NOP primitives.
        const bool is_filler = getcode(prim) == 0x00;
        if (!is_filler) {
            const size_t prim_addr = (size_t)prim;
            const GsOTPrimCode prim_code = (GsOTPrimCode)getcode(prim);
            char prim_name[32];
            GsGetOTPrimName(prim_name, prim_code);
            printf("--> <0x%lX: %s> ", prim_addr, prim_name);
        }
        prim = nextPrim(prim);
    }
    printf("--> <End>}\n");
}