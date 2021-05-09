#include "math.h"

#include <stdbool.h>

int GsIpow(int base, int exp) {
    int result = 1;
    while (true) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) break;
        base *= base;
    }
    return result;
}

long GsLpow(long base, long exp) {
    long result = 1;
    while (true) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) break;
        base *= base;
    }
    return result;
}

long GsAbsLong(long x) {
    // Assumes 32-bit long (is the case on PSX)
    long mask = x >> 31;
    x = mask ^ x;
    return (mask ^ x) - mask;
}