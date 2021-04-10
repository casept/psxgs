#ifndef PSXGS_TMD_H
#define PSXGS_TMD_H

// A TMD file's master header.
typedef struct {
    unsigned long id;
    unsigned long flags;
    unsigned long nobj;
} GsTMDHeader;

// A TMD object entry. Describes where to find the
// various components of an object.
typedef struct {
    // Pointer to beginning of vertex data
    unsigned long *vertop;
    // Number of vertices
    unsigned long vern;
    // Pointer to beginning of normal data
    unsigned long *nortop;
    // Number of normals
    unsigned long norn;
    // Pointer to beginning of TMD primitive packet data
    unsigned long *primtop;
    // Number of TMD primitive packets
    unsigned long primn;
    // Object scale
    signed long scale;
} GsTMDObject;

// Header of a TMD primitive packet.
// Not the same as a GPU packet, but a GPU packet is generated from this.
typedef struct {
    // Length of the generated GPU primitive packet in 32 bit machine words
    unsigned char olen;
    // Length of the TMD packet (without header) in 32 bit machine words
    unsigned char ilen;
    /* Flags for color gradation,
       double or single-facedness of poly,
       and lighting
    */
    unsigned char flag;
    // Type of primitive and primitive-specific flags
    unsigned char mode;
} GsTMDPacketHeader;

// Flat (untextured) triangle TMD primitive packet.
typedef struct {
    GsTMDPacketHeader hdr;
    // Red
    unsigned char r0;
    // Green
    unsigned char g0;
    // Blue
    unsigned char b0;
    // Identical to mode in the header
    unsigned char mode_copy;
    // Offset from start of object's normals to 0th normal
    unsigned short norm0;
    // Offset from start of object's vertices to 0th vertex
    unsigned short vert0;
    // Offset from start of object's vertices to 1st vertex
    unsigned short vert1;
    // Offset from start of object's vertices to 2nd vertex
    unsigned short vert2;
} GsTMDPF3;

// A vertex in the format used in TMD.
typedef struct {
    short vx;
    short vy;
    short vz;
} GsTMDVertex;

// The various TMD primitive types.
typedef enum {
    // Flat, untextured, solid colored triangle
    GS_TMD_PRIMITIVE_F3,
    // Unknown primitive
    GS_TMD_PRIMITIVE_UNKNOWN,
} GsTMDPrimitiveKind;
#endif