#include "tmd.h"

#include <psxgpu.h>
#include <stdbool.h>
#include <stdio.h>

#include "dbg.h"
#include "functions_pub.h"
#include "globals_pub.h"
#include "types_pub.h"

#define TMD_HDR_FLAG_REMAP_BIT 0x00000001

#define TMD_HDR_MODE_MASK 0xE0  // 0b11100000
#define TMD_MODE_POLY     0x20  // 0b00100000
#define TMD_MODE_LINE     0x40  // 0b01000000
#define TMD_MODE_SPRT     0x60  // 0b01100000
#define TMD_OPT_QUAD      0x8   // 0b00001000

void GsMapModelingData(unsigned long *p) {
    GsTMDHeader *hdr = (GsTMDHeader *)p;
    // Are the addresses in need of remap?
    if ((hdr->flags & TMD_HDR_FLAG_REMAP_BIT) == TMD_HDR_FLAG_REMAP_BIT) {
#ifdef DEBUG
        printf("[psxgs] TMD already remapped\n");
#endif
        return;
    }

    GsTMDObject *obj_table = (GsTMDObject *)(p + (sizeof(GsTMDHeader) / sizeof(unsigned long)));
#ifdef DEBUG
    printf("[psxgs] Remaping TMD (TMD starts at 0x%X)\n", (unsigned long)p);
    GsDumpTMDHeader(hdr);
#endif
    for (unsigned long i = 0; i < hdr->nobj; i++) {
        GsTMDObject *obj = obj_table + i;
        obj->nortop = (unsigned long *)((unsigned long)obj->nortop + (unsigned long)obj_table);
        obj->primtop = (unsigned long *)((unsigned long)obj->primtop + (unsigned long)obj_table);
        obj->vertop = (unsigned long *)((unsigned long)obj->vertop + (unsigned long)obj_table);
#ifdef DEBUG
        printf("[psxgs] New base address for TMD object: 0x%X\n", (unsigned long)obj_table);
#endif
    }

    // Mark addrs as having been remapped
    hdr->flags |= TMD_HDR_FLAG_REMAP_BIT;
}

// Return a pointer to the nth object in the given TMD.
inline static GsTMDObject *GsLookupTmdObj(const unsigned long *tmd, const size_t n) {
    unsigned long *tmd_after_hdr = (unsigned long *)((size_t)tmd + (size_t)sizeof(GsTMDHeader));
    GsTMDObject *obj_table = (GsTMDObject *)tmd_after_hdr;
    return obj_table + n;
}

// Return the nth vertex of the given TMD object.
inline static GsTMDVertex GsLookupTmdVert(const GsTMDObject *obj, const size_t n) {
    // TODO: Is it correct to assume that TMD stores the number of the vertex, not
    // an offset in bytes or sizeof(unsigned long)?
    const unsigned long *vert_data = obj->vertop + (2 * n);
    GsTMDVertex vert;
    vert.vx = vert_data[0] & 0x0000FFFF;
    vert.vy = (vert_data[0] & 0xFFFF0000) >> 16;
    vert.vz = vert_data[1] & 0x0000FFFF;
    return vert;
}

void GsLinkObject4(unsigned long tmd, GsDOBJ2 *obj_base, int n) {
    GsTMDObject *obj = GsLookupTmdObj((unsigned long *)tmd, (size_t)n);
    obj_base->tmd = (unsigned long *)obj;
}

// Parse header of a TMD primitive packet
inline static GsTMDPacketHeader GsParseTMDPacketHeader(const unsigned long hdr_int) {
    GsTMDPacketHeader hdr;
    hdr.mode = (hdr_int & 0xFF000000) >> 24;
    hdr.flag = (hdr_int & 0x00FF0000) >> 16;
    hdr.ilen = (hdr_int & 0x0000FF00) >> 8;
    hdr.olen = (hdr_int & 0x000000FF);
    return hdr;
}

// Parse an untextured triangle TMD primitive
inline static GsTMDPF3 GsParsePolyF3Primitive(const unsigned long *prim_data) {
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

// Sort an untextured triangle TMD primitive into the given OT
inline static void GsSortObject4PolyF3(const unsigned long *prim, const GsTMDObject *obj, GsOT *otp,
                                       __attribute__((unused)) const int shift) {
    GsTMDPF3 tmd_prim = GsParsePolyF3Primitive(prim);
#ifdef DEBUG
    GsDumpTMDPF3(&tmd_prim);
#endif
    // TODO: Perspective transform
    // TODO: Light calc
    // TODO: Implement polygon division (in scratchpad)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    POLY_F3 *gpu_packet = (POLY_F3 *)GsGetWorkBase();
#pragma GCC diagnostic pop
    // Copy the relevant attributes
    setPolyF3(gpu_packet);
    setRGB0(gpu_packet, tmd_prim.r0, tmd_prim.g0, tmd_prim.b0);
    const GsTMDVertex vert0 = GsLookupTmdVert(obj, tmd_prim.vert0);
    const GsTMDVertex vert1 = GsLookupTmdVert(obj, tmd_prim.vert1);
    const GsTMDVertex vert2 = GsLookupTmdVert(obj, tmd_prim.vert2);
    setXY3(gpu_packet, vert0.vx, vert0.vy, vert1.vx, vert1.vy, vert2.vx, vert2.vy);

    // Sort into OT
    // TODO: Is using org here correct?
    // TODO: Z-sort
    AddPrim((unsigned int *)otp->org, gpu_packet);
}

// Figure out what kind of primitive we're dealing with
static GsTMDPrimitiveKind GsParseTMDPrimitiveKind(const unsigned long *tmd_prim) {
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

void GsSortObject4(GsDOBJ2 *objp, GsOT *otp, int shift, __attribute__((unused)) unsigned long *scratch) {
    GsTMDObject *tmd = (GsTMDObject *)objp->tmd;
#ifdef DEBUG
    printf("[psxgs] Header of TMD object to be sorted:\n");
    GsDumpTMDStruct(tmd);
#endif

    // Keep track of where in the primitives section of the object we are
    const unsigned long *cursor = tmd->primtop;

    for (unsigned long i = 0; i < tmd->primn; i++) {
        switch (GsParseTMDPrimitiveKind(cursor)) {
            case GS_TMD_PRIMITIVE_F3:
                // Handle untextured flat triangle
                GsSortObject4PolyF3(cursor, tmd, otp, shift);
                break;
            case GS_TMD_PRIMITIVE_UNKNOWN:
            default:
                GsFatal("GsSortObject4(): Invalid primitive");
                break;
        }
        // Advance the cursor to the next primitive
        const GsTMDPacketHeader hdr = GsParseTMDPacketHeader(*cursor);
        cursor += (hdr.ilen + 1);  // +1 because ilen does not include the header.
    }
}
