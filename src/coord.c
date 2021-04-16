#include "functions_pub.h"
#include "globals_pub.h"
#include "types_pub.h"

void GsInitCoordinate2(GsCOORDINATE2 *super, GsCOORDINATE2 *base) {
    base->super = super;
    base->coord = GsIDMATRIX;
    super->sub = base;
}