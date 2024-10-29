/**
@file

This file contains the InitializePointToPoint() function.
*/

/* Standard includes. */
#include <complex>

/* Local includes. */
#include "../include/ilm.h"
#include "../include/Enums.h"

/**
@brief
Initialize parameters for point-to-point mode.

@param[in] f__mhz
Frequency, in MHz.

@param[in] pol
Polarization.
Either:
    0: POLARIZATION__HORIZONTAL
    1: POLARIZATION__VERTICAL

@param[in] epsilon
Relative permittivity.

@param[in] sigma
Conductivity.

@param[out] Z_g
Complex ground impedance.

*/
void InitializePointToPoint(
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    complex<double> *Z_g
) {
    // Complex relative permittivity.
    complex<double> ep_r = complex<double>(epsilon, 18000.0 * sigma / f__mhz);

    // [RLS, A-3].
    // Ground impedance (horizontal polarization).
    *Z_g = sqrt(ep_r - 1.0);
    // Adjust for vertical polarization.
    if (pol == POLARIZATION__VERTICAL)
        *Z_g = *Z_g / ep_r;
}