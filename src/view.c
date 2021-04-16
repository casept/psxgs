#include "dbg.h"
#include "functions_pub.h"
#include "globals_pub.h"
#include "types_pub.h"

int GsSetView2(GsVIEW2 *pv) {
    // TODO: Support other coordinate systems
    if (pv->super != &WORLD) {
        GsFatal("GsSetView2(): Coordinate systems other than WORLD not yet supported");
    }
    GsWSMATRIX = pv->view;
    return 0;
}