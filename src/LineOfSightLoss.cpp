#include "../include/ilm.h"

/*=============================================================================
 |
 |  Description:  Compute the loss in the line-of-sight region
 |
 |        Input:  s__meter          - Path distance, in meters
 |                h_e__meter[2]     - Terminal effective heights, in meters
 |                Z_g               - Complex surface transfer impedance
 |                delta_h__meter    - Terrain irregularity parameter
 |                m_d               - Diffraction slope
 |                A_ed              - Diffraction intercept
 |                d_ls__meter       - Maximum line-of-sight distance for
 |                                    a smooth earth, in meters
 |                f__mhz            - Frequency, in MHz
 |
 |      Outputs:  [None]
 |
 |      Returns:  A_los__db         - Loss, in dB
 |
 *===========================================================================*/
double LineOfSightLoss(double s__meter, double h_e__meter[2], complex<double> Z_g, 
    double delta_h__meter, double m_d, double A_ed, double d_ls__meter, double f__mhz)
{
    double delta_h_d__meter = TerrainRoughness(s__meter, delta_h__meter);

    // [RLS, A-67 & B-65]
    double sigma_h_s__meter = (delta_h__meter / 1.282) * exp(-pow(delta_h__meter, 0.25) / 2.0);

    // [RLS, A-1]
    double c = 299792458;  // speed of light, m/s
    double k = 2 * PI * (f__mhz * 1e6) / c;

    // [RLS, A-65 & B-63]
    double sin_psi = (h_e__meter[0] + h_e__meter[1]) / sqrt(pow(s__meter, 2) + pow(h_e__meter[0] + h_e__meter[1], 2));

    // [RLS, A-66 & B-64]
    complex<double> R_e = (sin_psi - Z_g) / (sin_psi + Z_g) * exp(-k * sigma_h_s__meter * sin_psi);

    // [RLS, A-69 & B-67]
    double q = pow(R_e.real(), 2) + pow(R_e.imag(), 2);
    if (q < 0.25 || q < sin_psi)
        R_e = R_e * sqrt(sin_psi / q);

    // [RLS, A-68 & B-66]
    double delta_phi = 2.0 * k * h_e__meter[0] * h_e__meter[1] / s__meter;

    // [RLS, A-70 & B-68]
    if (delta_phi > PI / 2.0)
        delta_phi = PI - pow(PI / 2.0, 2) / delta_phi;

    // [RLS, A-71 & B-69]
    complex<double> rr = complex<double>(cos(delta_phi), -sin(delta_phi)) + R_e;
    double A_t__db = -10 * log10(pow(rr.real(), 2) + pow(rr.imag(), 2));

    // [RLS, A-64 & B-62]
    double A_d__db = A_ed + m_d * s__meter;

    // [RLS, A-63 & B-61]
    double D_1 = 47.7;
    double D_2 = 10e3;
    double w = 1 / (1 + D_1 * k * delta_h__meter / MAX(D_2, d_ls__meter));

    // [RLS, A-62 & B-60]
    double A_los__db = (1 - w) * A_d__db + w * A_t__db;

    return A_los__db;
}