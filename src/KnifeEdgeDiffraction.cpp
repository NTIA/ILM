/**
@file

This file contains the KnifeEdgeDiffraction() function.
*/

/* Standard includes. */
#include <cmath>

/* Local includes. */
#include "../include/ilm.h"

/**
@brief
Compute the knife-edge diffraction loss.

@param[in] d__meter
Distance of interest, in meters.

@param[in] f__mhz
Frequency, in MHz.

@param[in] theta_los
Angular distance of line-of-sight region.

@param[in] d_hzn__meter
Horizon distances, in meters.

@return A_k__db
Knife-edge diffraction loss, in dB.

*/
double KnifeEdgeDiffraction(
    double d__meter,
    double f__mhz,
    double theta_los,
    double d_hzn__meter[2]
) {
    // Maximum line-of-sight distance for actual path.
    double d_ML__meter = d_hzn__meter[0] + d_hzn__meter[1];

    // Angular distance of diffraction region [Algorithm, Eqn 4.12].
    double theta_nlos = d__meter / a_m__meter - theta_los;

    // Diffraction distance, in meters.
    double d_nlos__meter = d__meter - d_ML__meter;


    // [TN101, Eqn I.7].
    // 1 / (4 pi) = 0.0795775
    double v_1 = 0.0795775 * (f__mhz / 47.7) * pow(theta_nlos, 2) * d_hzn__meter[0] * d_nlos__meter / (d_nlos__meter + d_hzn__meter[0]);
    double v_2 = 0.0795775 * (f__mhz / 47.7) * pow(theta_nlos, 2) * d_hzn__meter[1] * d_nlos__meter / (d_nlos__meter + d_hzn__meter[1]);

    // [RLS, A-26& B-24].
    double A_k__db = FresnelIntegral(v_1) + FresnelIntegral(v_2);

    return A_k__db;
}