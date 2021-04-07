#ifndef PSXGS_DBG_H
#define PSXGS_DBG_H

/* Various utility functions to aid in debugging. */

#include "tmd.h"
#include "types_pub.h"

// Print the given string and terminate.
void GsFatal(const char *msg);

// Print the data structure to stdout
void GsDumpTMDStruct(const GsTMDObject *t);

// Print the data structure to stdout
void GsDumpTMDHeader(const GsTMDHeader *h);

// Print the data structure to stdout
void GsDumpTMDPF3(const GsTMDPF3 *f3);

// Print the data structure to stdout
void GsDumpTMDPacketHeader(const GsTMDPacketHeader *hdr);

#endif