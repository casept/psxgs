#include <psxgpu.h>
#include <stdbool.h>

#include "functions_pub.h"
#include "globals_pub.h"
#include "types_pub.h"

// Integer power function, because apparently it's acceptable for a systems language to lack one
inline static int GsIpow(int base, int exp) {
    int result = 1;
    while (true) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) break;
        base *= base;
    }
    return result;
}

void GsClearOT(unsigned short offset, unsigned short point, GsOT* otp) {
    otp->offset = offset;
    otp->point = point;
    ClearOTagR((unsigned int*)otp->org, GsIpow(2, (int)otp->length));
    // TODO: Point to last array elem instead?
    otp->tag = otp->org;
}

void GsDrawOT(GsOT* ot) { DrawOTag((unsigned int*)ot->org); }