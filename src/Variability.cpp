/* Standard includes. */
#include <cmath>

/* Local includes. */
#include "../include/ilm.h"

/*=============================================================================
 |
 |  Description:  Compute the variability
 |
 |        Input:  p              - Location percentage, 0 < location < 1
 |                delta_h__meter - Terrain irregularity parameter
 |                f__mhz         - Frequency, in MHz
 |                d__meter       - Path distance, in meters
 |                A_ref__db      - Reference attenuation, in dB
 |
 |      Outputs:  [None]
 |
 |      Returns:  A_ref__db      - Reference attenuation, with variability,
 |                                 in dB
 |
 *===========================================================================*/
double Variability(double p, double delta_h__meter, 
    double f__mhz, double d__meter, double A_ref__db)
{
    // [RLS, A-1 & B-1]
    double c = 299792458;  // speed of light, m/s
    double k = 2 * PI * (f__mhz * 1e6) / c;

    // [RLS, A-72 & B-70]
    double delta_h_d__meter = delta_h__meter * (1.0 - 0.8 * exp(-d__meter / 50e3));

    // [RLS, A-73 & B-71]
    double sigma = 10.0 * k * delta_h_d__meter / (k * delta_h_d__meter + 13.0);

    // [RLS, A-74 & B-72]
    double z = InverseComplementaryCumulativeDistributionFunction(p);

    // [RLS, A-75 & B-73]
    A_ref__db = A_ref__db + sigma * z;

    // [Algorithm, Eqn 52]
    if (A_ref__db < 0.0)
        A_ref__db = A_ref__db * (29.0 - A_ref__db) / (29.0 - 10.0 * A_ref__db);

    return A_ref__db;
}