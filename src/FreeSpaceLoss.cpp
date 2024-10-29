/**
@file

This file contains FreeSpaceLoss() which returns the free space basic
transmission loss in dB.
*/

/* Standard includes. */
#include <cmath>

/* Local includes. */
#include "./include/ilm.h"

/** |
@brief
Return the free space basic transmission loss in dB.

@param[in] d__meter
Path distance, in meters.

@param[in] f__mhz
Frequency, in MHz.

@return A_fs__db
Free space basic transmission loss, in dB.

*/
double FreeSpaceLoss(
    double d__meter,
    double f__mhz
) {
    return 32.45 + 20.0 * log10(f__mhz) + 20.0 * log10(d__meter / 1000.0);
}