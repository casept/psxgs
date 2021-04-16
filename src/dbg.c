#include "dbg.h"

#include <stdbool.h>
#include <stdio.h>

#include "tmd.h"
#include "types_pub.h"

void GsFatal(const char *msg) {
    printf("[psxgs] Fatal psxgs error: %s\n", msg);
    while (true) {
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

void GsDumpTMDPacketHeader(const GsTMDPacketHeader *hdr) {
    printf("[psxgs] GsTMDPacketHeader: {hdr: {olen: %u, ilen: %u, flag: %u, mode: %u}}\n", hdr->olen, hdr->ilen,
           hdr->flag, hdr->mode);
}