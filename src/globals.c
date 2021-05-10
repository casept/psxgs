#include <psxgpu.h>
#include <psxgte.h>
#include <stdio.h>

#include "types_pub.h"

DRAWENV GsDRAWENV;
DISPENV GsDISPENV;

PACKET *GsOUT_PACKET_P = NULL;
PACKET *GsOUT_PACKET_CURSOR = NULL;

short PSDBASEX[2] = {0, 0};
short PSDBASEY[2] = {0, 0};
short PSDIDX = 0;
short PSDCNT = 0;

// TODO: What is a parallel transfer volume?
MATRIX GsIDMATRIX = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}, {1, 1, 1}};
MATRIX GsWSMATRIX = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}, {1, 1, 1}};

// TODO: Should the matrix be ID matrix instead?
#define WORLD_INITIAL_MATRIX                           \
    {                                                  \
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, { 1, 1, 1 } \
    }
GsCOORDINATE2 WORLD = {0, WORLD_INITIAL_MATRIX, WORLD_INITIAL_MATRIX, NULL, NULL, NULL};