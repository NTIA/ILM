/**
@file

This file contains the function DiffractionLoss() to calculate diffraction loss
in the ILM.
*/

/* Standard includes. */
#include <cmath>
#include <complex>
#include <utility>

/* Local includes. */
#include "./include/ilm.h"

/**
@brief
Compute the diffraction loss at a specified distance

@param[in] d__meter
Path distance, in meters.

@param[in] d_hzn__meter
Horizon distances, in meters.

@param[in] h_e__meter
Effective terminal heights, in meters.

@param[in] Z_g
Complex ground impedance.

@param[in] delta_h__meter
Terrain irregularity parameter, in meters.

@param[in] h__meter
Terminal heights, in meters.

@param[in] theta_los
Angular distance of line-of-sight region.

@param[in] f__mhz
Frequency, in MHz.

@return A_d__db
Diffraction loss, in dB.

*/
double DiffractionLoss(
    double d__meter,
    double d_hzn__meter[2],
    double h_e__meter[2],
    std::complex<double> Z_g,
    double delta_h__meter,
    double h__meter[2],
    double theta_los,
    double f__mhz
) {
    double A_k__db = KnifeEdgeDiffraction(
        d__meter,
        f__mhz,
        theta_los,
        d_hzn__meter
    );

    double A_r__db = SmoothMoonDiffraction(
        d__meter,
        f__mhz,
        theta_los,
        d_hzn__meter,
        h_e__meter,
        Z_g
    );

    double delta_h_d__meter = TerrainRoughness(
        d__meter,
        delta_h__meter
    );

    // [RLS, A-25 & B-23].
    double term1 = sqrt((h_e__meter[0] * h_e__meter[1]) / (h__meter[0] * h__meter[1]));
    double d_l__meter = d_hzn__meter[0] + d_hzn__meter[1];
    double q = (term1 + (-theta_los * a_m__meter + d_l__meter) / d__meter) * std::min(delta_h_d__meter * f__mhz / 47.7, 1000.0);

    // [RLS, A-24 & B-22].
    double w = 1.0 / (1.0 + 0.1 * sqrt(q));

    // [RLS, A-23 & B-21].
    double A_d__db = w * A_r__db + (1.0 - w) * A_k__db ;

    return A_d__db;
}