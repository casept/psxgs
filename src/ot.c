#include <psxgpu.h>
#include <stdbool.h>

#include "functions_pub.h"
#include "globals_pub.h"
#include "math.h"
#include "types_pub.h"

void GsClearOT(unsigned short offset, unsigned short point, GsOT* otp) {
    otp->offset = offset;
    otp->point = point;
    const int actual_length = GsIpow(2, (int)otp->length);
    ClearOTagR((unsigned int*)otp->org, actual_length);
    // Because the table is reversed, we start drawing at the last entry
    otp->tag = otp->org + (actual_length - 1);
}

void GsDrawOT(GsOT* ot) { DrawOTag((unsigned int*)ot->tag); }