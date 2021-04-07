#include <psxgpu.h>
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