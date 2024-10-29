/**
@file

This file contains the SigmaHFunction() function.
*/

/* Standard includes. */
#include <cmath>

/* Local includes. */
#include "../include/ilm.h"

/**
@brief
Return sigma_h.

Return the RMS deviation of terrain and terrain clutter within the limits of
the first Fresnel zone in the dominant reflecting plane.

@param[in] delta_h__meter
Terrain irregularity parameter.

@return
sigma_h, in meters.

*/
double SigmaHFunction(
    double delta_h__meter
) {
    // [ERL 79-ITS 67, Eqn 3.6a].
    return 0.78 * delta_h__meter * exp(-0.5 * pow(delta_h__meter, 0.25));
}