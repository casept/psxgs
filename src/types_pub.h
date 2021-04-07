#ifndef PSXGS_TYPES_PUB_H_
#define PSXGS_TYPES_PUB_H_

// IMPLEMENTATION DETAIL. DO NOT IMPORT DIRECTLY.
// This header describes the various libgs/psxgs datatypes.

#include <psxgte.h>

// Retains model position between animation frames (?).
typedef struct {
    VECTOR scale;
    SVECTOR rotate;
    VECTOR trans;
} GsCOORD2PARAM;

// A coordinate system (for example for a 3D object or the WORLD)
typedef struct GsCOORDINATE2 {
    // Marker for invalidating cached calculations in case coord is changed
    unsigned long flg;
    // Actual coordinates (?)
    MATRIX coord;
    // Result of multiplying this with WORLD
    MATRIX workm;
    // Rotation, scale, transform params (Only relevant for TOD animation)
    GsCOORD2PARAM *param;
    // Pointer to the superior coordinate system (typically to WORLD)
    struct GsCOORDINATE2 *super;
    // Unused, only here to remain compatible with Sony's definition
    struct GsCOORDINATE2 *sub;
} GsCOORDINATE2;

// A handle for manipulating a 3D model object.
typedef struct {
    // TODO: Use this
    unsigned long attribute;
    //
    GsCOORDINATE2 *coord2;
    unsigned long *tmd;
    unsigned long id;
} GsDOBJ2;

typedef struct {
    // The 24 LSB of pointer to next OT entry (which is also a GsOT_TAG)
    unsigned p : 24;
    // Number of 32-bit words in current GPU primitive packet
    unsigned char num : 8;
} GsOT_TAG;

// Header of a primitive ordering table.
typedef struct {
    // This field's name is a bit misleading. It actually refers to the Z-precision
    // of the table, not the actual number of entries.
    unsigned long length;
    // First primitive to be drawn
    GsOT_TAG *org;
    // Unused, only here to remain compatible with Sony's definition
    unsigned long offset;
    // Specifies the priority of where to sort this table
    unsigned long point;
    // Next primitive to be drawn
    GsOT_TAG *tag;
} GsOT;

// Supposed to be a GPU primitive packet, but Sony defines it like this
// (which doesn't make sense, as packets are variable size and way larger than a char)
typedef unsigned char PACKET;

#endif