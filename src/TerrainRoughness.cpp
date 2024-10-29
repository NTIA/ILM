/**
@file

This file contains the TerrainRoughness() function.
*/

/* Standard includes. */
#include <cmath>

/* Local includes. */
#include "../include/ilm.h"

/**
@brief
Compute delta_h_d.

@param[in] d__meter
Path distance, in meters.

@param[in] delta_h__meter
Terrain irregularity parameter.

@return
delta_h_d: Terrain irregularity of path.

*/
double TerrainRoughness(
    double d__meter,
    double delta_h__meter
) {
    // [RLS, A-14].
    return delta_h__meter * (1.0 - 0.8 * exp(-d__meter / 50.0E3));
}