#include <psxgte.h>
#include <stdio.h>

#include "dbg.h"
#include "tmd.h"
#include "types_pub.h"

#define TMD_HDR_FLAG_REMAP_BIT 0x00000001

#define TMD_HDR_MODE_MASK 0xE0  // 0b11100000
#define TMD_MODE_POLY     0x20  // 0b00100000
#define TMD_MODE_LINE     0x40  // 0b01000000
#define TMD_MODE_SPRT     0x60  // 0b01100000
#define TMD_OPT_QUAD      0x8   // 0b00001000

// Return a pointer to the nth object in the given TMD.
GsTMDObject *GsLookupTmdObj(const unsigned long *tmd, const size_t n) {
    unsigned long *tmd_after_hdr = (unsigned long *)((size_t)tmd + (size_t)sizeof(GsTMDHeader));
    GsTMDObject *obj_table = (GsTMDObject *)tmd_after_hdr;
    return obj_table + n;
}

SVECTOR GsLookupTmdVert(const GsTMDObject *obj, const size_t n) {
    const unsigned long *vert_data = obj->vertop + (2 * n);
    SVECTOR vert;
    vert.vx = vert_data[0] & 0x0000FFFF;
    vert.vy = (vert_data[0] & 0xFFFF0000) >> 16;
    vert.vz = vert_data[1] & 0x0000FFFF;
    vert.pad = 0;
    return vert;
}

SVECTOR GsLookupTmdNorm(const GsTMDObject *obj, const size_t n) {
    const unsigned long *norm_data = obj->nortop + (2 * n);
    SVECTOR norm;
    norm.vx = norm_data[0] & 0x0000FFFF;
    norm.vy = (norm_data[0] & 0xFFFF0000) >> 16;
    norm.vz = norm_data[1] & 0x0000FFFF;
    norm.pad = 0;
    return norm;
}

GsTMDPacketHeader GsParseTMDPacketHeader(const unsigned long hdr_int) {
    GsTMDPacketHeader hdr;
    hdr.mode = (hdr_int & 0xFF000000) >> 24;
    hdr.flag = (hdr_int & 0x00FF0000) >> 16;
    hdr.ilen = (hdr_int & 0x0000FF00) >> 8;
    hdr.olen = (hdr_int & 0x000000FF);
    return hdr;
}

GsTMDPF3 GsParsePolyF3Primitive(const unsigned long *prim_data) {
    GsTMDPF3 prim;
    prim.hdr = GsParseTMDPacketHeader(prim_data[0]);
    const unsigned long color = prim_data[1];
    prim.b0 = (color & 0x00FF0000) >> 16;
    prim.g0 = (color & 0x0000FF00) >> 8;
    prim.r0 = (color & 0x000000FF);
    prim.vert0 = (prim_data[2] & 0xFFFF0000) >> 16;
    prim.norm0 = (prim_data[2] & 0x0000FFFF);
    prim.vert1 = (prim_data[3] & 0x0000FFFF);
    prim.vert2 = (prim_data[3] & 0xFFFF0000) >> 16;
    return prim;
}

GsTMDPrimitiveKind GsParseTMDPrimitiveKind(const unsigned long *tmd_prim) {
    const GsTMDPacketHeader hdr = GsParseTMDPacketHeader(*tmd_prim);
#ifdef DEBUG
    printf("Determining kind of TMD packet with header: ");
    GsDumpTMDPacketHeader(&hdr);
#endif

    // What sort of TMD primitive are we dealing with?
    switch (hdr.mode & TMD_HDR_MODE_MASK) {
        case TMD_MODE_POLY:
            if ((hdr.mode & TMD_OPT_QUAD) == TMD_OPT_QUAD) {
                GsFatal("TMD: quad primitive not implemented");
            } else {
                // TODO: Check if flat
                // TODO: Check if textured etc.
                return GS_TMD_PRIMITIVE_F3;
            }
            break;
        case TMD_MODE_LINE:
            GsFatal("TMD: line primitive not implemented");
            break;
        case TMD_MODE_SPRT:
            GsFatal("TMD: sprite primitive not implemented");
            break;
        default:
            printf("[psxgs] Header mode field: 0x%X\n", hdr.mode);
            GsFatal("TMD: invalid primitive");
            break;
    }
    return GS_TMD_PRIMITIVE_UNKNOWN;
}