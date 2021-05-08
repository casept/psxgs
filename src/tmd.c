#include "tmd.h"

#include <inline_c.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <stdio.h>

#include "dbg.h"
#include "functions_pub.h"
#include "globals_pub.h"
#include "math.h"
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

void GsLinkObject4(unsigned long tmd, GsDOBJ2 *obj_base, int n) {
    GsTMDObject *obj = GsLookupTmdObj((unsigned long *)tmd, (size_t)n);
    obj_base->tmd = (unsigned long *)obj;
}

// Sort an untextured flat triangle TMD primitive into the given OT
inline static void GsSortObject4PolyF3(__attribute__((unused)) const GsCOORDINATE2 *cs, const unsigned long *prim,
                                       const GsTMDObject *obj, GsOT *otp, __attribute__((unused)) const int shift) {
    GsTMDPF3 tmd_prim = GsParsePolyF3Primitive(prim);
#ifdef DEBUG
    GsDumpTMDPF3(&tmd_prim);
#endif

    // TODO: Implement polygon division (in scratchpad)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    POLY_F3 *pol3 = (POLY_F3 *)GsGetWorkBase();
#pragma GCC diagnostic pop
    setPolyF3(pol3);

    // TODO: Light calc
    gte_ldrgb(&tmd_prim.r0);

    // Configure GTE
    MATRIX mtx;
    SVECTOR rot = {1, 1, 1, 0};
    RotMatrix(&rot, &mtx);
    // FIXME: Should probably be based on coord system in relation to world
    VECTOR pos = {0, 0, 0};
    TransMatrix(&mtx, &pos);
    gte_SetRotMatrix(&mtx);
    gte_SetTransMatrix(&mtx);

    // Load verts into GTE
    SVECTOR vert0 = GsLookupTmdVert(obj, tmd_prim.vert0);
    SVECTOR vert1 = GsLookupTmdVert(obj, tmd_prim.vert1);
    SVECTOR vert2 = GsLookupTmdVert(obj, tmd_prim.vert2);
    gte_ldv3(&vert0, &vert1, &vert2);

    // Rotate, translate, perspective transform
    gte_rtpt();

    // Apply the transformed vertices to primitive
    gte_stsxy3_f3(pol3);

    // Apply the transformed color to primitive
    gte_ncs();
    gte_strgb(&pol3->r0);

    // For some weird reason, the primitive code is corrupted by GTE writeback. Reinit to workaround.
    setPolyF3(pol3);
    // Z-Sort into OT
    gte_avsz3();
    long avg_z = 0;
    gte_stotz(&avg_z);
    const long z = avg_z >> shift;
    addPrim((unsigned int *)&otp->org[z], pol3);
}

// Sort an untextured flat quad TMD primitive into the given OT
inline static void GsSortObject4PolyF4(__attribute__((unused)) const GsCOORDINATE2 *cs, const unsigned long *prim,
                                       const GsTMDObject *obj, GsOT *otp, __attribute__((unused)) const int shift) {
    GsTMDPF4 tmd_prim = GsParsePolyF4Primitive(prim);
#ifdef DEBUG
    GsDumpTMDPF4(&tmd_prim);
#endif

    // TODO: Implement polygon division (in scratchpad)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    POLY_F4 *pol4 = (POLY_F4 *)GsGetWorkBase();
#pragma GCC diagnostic pop
    setPolyF4(pol4);

    // TODO: Light calc
    gte_ldrgb(&tmd_prim.r0);

    // Configure GTE
    MATRIX mtx;
    SVECTOR rot = {1, 1, 1, 0};
    RotMatrix(&rot, &mtx);
    // FIXME: Should probably be based on coord system in relation to world
    VECTOR pos = {0, 0, 0};
    TransMatrix(&mtx, &pos);
    gte_SetRotMatrix(&mtx);
    gte_SetTransMatrix(&mtx);

    // Load verts into GTE
    SVECTOR vert0 = GsLookupTmdVert(obj, tmd_prim.vert0);
    SVECTOR vert1 = GsLookupTmdVert(obj, tmd_prim.vert1);
    SVECTOR vert2 = GsLookupTmdVert(obj, tmd_prim.vert2);
    SVECTOR vert3 = GsLookupTmdVert(obj, tmd_prim.vert3);
    gte_ldv3(&vert0, &vert1, &vert2);

    // Rotate, translate, perspective transform
    gte_rtpt();

    // Apply the transformed vertices to primitive
    gte_stsxy3_f3(pol4);

    // For some weird reason, the primitive code is corrupted by GTE writeback. Reinit to workaround.
    setPolyF4(pol4);
    // Calculate Z based on first 3 vertices
    gte_avsz4();
    long avg_z = 0;
    gte_stotz(&avg_z);
    const long z = avg_z >> shift;

    // Process remaining vert
    gte_ldv0(&vert3);
    gte_rtps();
    gte_stsxy(&pol4->x3);

    // Process color
    gte_ncs();
    gte_strgb(&pol4->r0);

    // For some weird reason, the primitive code is corrupted by GTE writeback. Reinit to workaround.
    setPolyF4(pol4);

    addPrim((unsigned int *)&otp->org[z], pol4);
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
                GsSortObject4PolyF3(objp->coord2, cursor, tmd, otp, shift);
                break;
            case GS_TMD_PRIMITIVE_F4:
                // Handle untextured flat quad
                GsSortObject4PolyF4(objp->coord2, cursor, tmd, otp, shift);
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
