/* Standard includes. */
#include <cmath>
#include <complex>

/* Local includes. */
#include "../include/ilm.h"

/*=============================================================================
 |
 |  Description:  Compute the diffraction loss at a specified distance
 |
 |        Input:  d__meter        - Path distance, in meters
 |                d_hzn__meter[2] - Horizon distances, in meters
 |                h_e__meter[2]   - Effective terminal heights, in meters
 |                Z_g             - Complex ground impedance
 |                delta_h__meter  - Terrain irregularity parameter, in meters
 |                h__meter[2]     - Terminal heights, in meters
 |                theta_los       - Angular distance of line-of-sight region
 |                f__mhz          - Frequency, in MHz
 |
 |      Outputs:  [None]
 |
 |      Returns:  A_d__db        - Diffraction loss, in dB
 |
 *===========================================================================*/
double DiffractionLoss(double d__meter, double d_hzn__meter[2], double h_e__meter[2], complex<double> Z_g, 
    double delta_h__meter, double h__meter[2], double theta_los, double f__mhz)
{
    double A_k__db = KnifeEdgeDiffraction(d__meter, f__mhz, theta_los, d_hzn__meter);

    double A_r__db = SmoothMoonDiffraction(d__meter, f__mhz, theta_los, d_hzn__meter, h_e__meter, Z_g);

    double delta_h_d__meter = TerrainRoughness(d__meter, delta_h__meter);

    // [RLS, A-25 & B-23]
    double term1 = sqrt((h_e__meter[0] * h_e__meter[1]) / (h__meter[0] * h__meter[1]));
    double d_l__meter = d_hzn__meter[0] + d_hzn__meter[1];
    double q = (term1 + (-theta_los * a_m__meter + d_l__meter) / d__meter) * MIN(delta_h_d__meter * f__mhz / 47.7, 1000);

    // [RLS, A-24 & B-22]
    double w = 1 / (1 + 0.1 * sqrt(q));

    // [RLS, A-23 & B-21]
    double A_d__db = w * A_r__db + (1.0 - w) * A_k__db ;

    return A_d__db;
}