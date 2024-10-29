/**
@file

This file contains the Variability() function.
*/

/* Standard includes. */
#define _USE_MATH_DEFINES
#include <cmath>

/* Local includes. */
#include "./include/ilm.h"

/**
@brief
Compute the variability.

@param[in] p
Location percentage, 0 < location < 1.

@param[in] delta_h__meter
Terrain irregularity parameter.

@param[in] f__mhz
Frequency, in MHz.

@param[in] d__meter
Path distance, in meters.

@param[in] A_ref__db
Reference attenuation, in dB.

@return A_ref__db
Reference attenuation, with variability, in dB.

*/
double Variability(
    double p,
    double delta_h__meter,
    double f__mhz,
    double d__meter,
    double A_ref__db
) {
    // Speed of light, m/s.
    double c = 299792458.0;
    // [RLS, A-1 & B-1].
    double k = 2.0 * M_PI * (f__mhz * 1.0E6) / c;

    // [RLS, A-72 & B-70].
    double delta_h_d__meter = delta_h__meter * (1.0 - 0.8 * exp(-d__meter / 50.0E3));

    // [RLS, A-73 & B-71].
    double sigma = 10.0 * k * delta_h_d__meter / (k * delta_h_d__meter + 13.0);

    // [RLS, A-74 & B-72].
    double z = InverseComplementaryCumulativeDistributionFunction(p);

    // [RLS, A-75 & B-73].
    A_ref__db = A_ref__db + sigma * z;

    // [Algorithm, Eqn 52].
    if (A_ref__db < 0.0)
        A_ref__db = A_ref__db * (29.0 - A_ref__db) / (29.0 - 10.0 * A_ref__db);

    return A_ref__db;
}