/**
@file

This file contains the LongleyRice() function.
*/

/* Standard includes. */
#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>
#include <cstdlib>
#include <utility>

/* Local includes. */
#include "./include/Enums.h"
#include "./include/Errors.h"
#include "./include/ilm.h"
#include "./include/Warnings.h"

/**
@brief
Dim of x and y.
*/
#define DIM(x, y) (((x) > (y)) ? (x - y) : (0))

/**
@brief
The ratio 1/3.
*/
#define THIRD 1.0 / 3.0

/**
@brief
Compute the reference attenuation, using the Longley-Rice method.

@param[in] theta_hzn
Terminal horizon angles.

@param[in] f__mhz
Frequency, in MHz.

@param[in] Z_g
Complex surface transfer impedance.

@param[in] d_hzn__meter
Terminal horizon distances, in meters.

@param[in] h_e__meter
Effective terminal heights, in meters.

@param[in] delta_h__meter
Terrain irregularity parameter.

@param[in] h__meter
Terminal structural heights, in meters.

@param[in] d__meter
Path distance, in meters.

@param[out] A_ref__db
Reference attenuation, in dB.

@param[out] warnings
Warning flags.

@param[out] propmode
Mode of propagation value.

@return error
Error code.

*/
int LongleyRice(
    double theta_hzn[2],
    double f__mhz,
    std::complex<double> Z_g,
    double d_hzn__meter[2],
    double h_e__meter[2],
    double delta_h__meter,
    double h__meter[2],
    double d__meter,
    double *A_ref__db,
    long *warnings,
    int *propmode
) {
    // [RLS, A-8 & B-8].
    double d_hzn_s__meter[2];
    for (int i = 0; i < 2; i++)
        d_hzn_s__meter[i] = sqrt(2.0 * h_e__meter[i] * a_m__meter);

    // [RLS, A-11].
    double d_ls__meter = d_hzn_s__meter[0] + d_hzn_s__meter[1];

    // [RLS, A-12].
    double d_l__meter = d_hzn__meter[0] + d_hzn__meter[1];

    // Check validity of small angle approximation.
    if (abs(theta_hzn[0]) > 200.0E-3)
        *warnings |= WARN__TX_HORIZON_ANGLE;
    if (abs(theta_hzn[1]) > 200.0E-3)
        *warnings |= WARN__RX_HORIZON_ANGLE;

    if (d_hzn__meter[0] < 0.1 * d_hzn_s__meter[0])
        *warnings |= WARN__TX_HORIZON_DISTANCE_1;
    if (d_hzn__meter[1] < 0.1 * d_hzn_s__meter[1])
        *warnings |= WARN__RX_HORIZON_DISTANCE_1;

    if (d_hzn__meter[0] > 3.0 * d_hzn_s__meter[0])
        *warnings |= WARN__TX_HORIZON_DISTANCE_2;
    if (d_hzn__meter[1] > 3.0 * d_hzn_s__meter[1])
        *warnings |= WARN__RX_HORIZON_DISTANCE_2;

    // Check ground impedance.
    if (Z_g.real() <= abs(Z_g.imag()))
        return ERROR__GROUND_IMPEDANCE;

    // Speed of light, m/s.
    double c = 299792458.0;
    // [RLS, A-1 & B-1].
    double k = 2.0 * M_PI * (f__mhz * 1.0E6) / c;

    // [RLS, A-20 & B-18].
    double X_ae__meter = pow(k / pow(a_m__meter, 2), -THIRD);

    // [RLS, A-16 & B-14].
    double d_3__meter = std::max(d_ls__meter, d_l__meter + 1.3787 * X_ae__meter);
    // [RLS, A-17 & B-15].
    double d_4__meter = d_3__meter + 2.7574 * X_ae__meter;

    // [RLS, A-18 & B-16].
    double A_3__db = DiffractionLoss(
        a_m__meter,
        d_3__meter,
        d_hzn__meter,
        h_e__meter,
        Z_g,
        delta_h__meter,
        h__meter,
        d_ls__meter,
        f__mhz
    );
    // [RLS, A-19 & B-17].
    double A_4__db = DiffractionLoss(
        a_m__meter,
        d_4__meter,
        d_hzn__meter,
        h_e__meter,
        Z_g,
        delta_h__meter,
        h__meter,
        d_ls__meter,
        f__mhz
    );

    // [RLS, A-21 & B-19].
    double m_d = (A_4__db - A_3__db) / (d_4__meter - d_3__meter);
    // [RLS, A-22 & B-20].
    double A_ed__db = A_3__db - m_d * d_3__meter;

    double d_min__meter = abs(h_e__meter[0] - h_e__meter[1]) / 200.0E-3;

    if (d__meter < d_min__meter)
        *warnings |= WARN__PATH_DISTANCE_TOO_SMALL_1;
    if (d__meter < 1.0E3)
        *warnings |= WARN__PATH_DISTANCE_TOO_SMALL_2;
    if (d__meter > 1000.0E3)
        *warnings |= WARN__PATH_DISTANCE_TOO_BIG_1;
    if (d__meter > 2000.0E3)
        *warnings |= WARN__PATH_DISTANCE_TOO_BIG_2;

    if (d__meter < d_ls__meter)
    {
        // [RLS, A-35 & B-33].
        double d_2__meter = d_ls__meter;

        // [RLS, A-36 & B-34].
        double A_2__db = A_ed__db + m_d * d_ls__meter;

        double d_0__meter;
        double d_1__meter;

        if (A_ed__db >= 0.0)  // [RLS, A.1.5, CASE 1].
        {
            // [RLS, A-37 & B-35].
            d_0__meter = std::min(0.5 * d_l__meter, 1.908 * k * h_e__meter[0] * h_e__meter[1]);
            // [RLS, A-38 & B-36].
            d_1__meter = 3.0 / 4.0 * d_0__meter + d_l__meter / 4.0;
        }
        else
        {
            // [RLS, A-47].
            d_0__meter = 1.908 * k * h_e__meter[0] * h_e__meter[1];
            // [RLS, A-48].
            d_1__meter = std::max(-A_ed__db / m_d, d_l__meter / 4.0);
        }

        double A_1__db = LineOfSightLoss(
            d_1__meter,
            h_e__meter,
            Z_g,
            delta_h__meter,
            m_d,
            A_ed__db,
            d_ls__meter,
            f__mhz
        );

        bool flag = false;

        double kHat_1 = 0.0;
        double kHat_2 = 0.0;

        if (d_0__meter < d_1__meter)
        {
            double A_0__db = LineOfSightLoss(
                d_0__meter,
                h_e__meter,
                Z_g,
                delta_h__meter,
                m_d,
                A_ed__db,
                d_ls__meter,
                f__mhz
            );

            double term1 = log(d_ls__meter / d_0__meter);

            // [ERL 79-ITS 67, Eqn 3.20].
            kHat_2 = std::max(0.0, ((d_ls__meter - d_0__meter) * (A_1__db - A_0__db) - (d_1__meter - d_0__meter) * (A_2__db - A_0__db)) / ((d_ls__meter - d_0__meter) * log(d_1__meter / d_0__meter) - (d_1__meter - d_0__meter) * term1));

            flag = A_ed__db > 0.0 || kHat_2 > 0.0;

            if (flag)
            {
                // [RLS, A-42 & A-52 & B-40 & B-50].
                kHat_1 = (A_2__db - A_0__db - kHat_2 * term1) / (d_2__meter - d_0__meter);

                if (kHat_1 < 0.0)
                {
                    kHat_1 = 0.0;

                    // [RLS, A-54 & B-42].
                    kHat_2 = DIM(A_2__db, A_0__db) / term1;

                    if (kHat_2 == 0.0)
                        kHat_1 = m_d;
                }
            }
        }

        if (!flag)
        {
            kHat_1 = DIM(A_2__db, A_1__db) / (d_ls__meter - d_1__meter);
            kHat_2 = 0.0;

            if (kHat_1 == 0.0)
                kHat_1 = m_d;
        }

        double A_o__db = A_2__db - kHat_1 * d_ls__meter - kHat_2 * log(d_ls__meter);

        // [ERL 79-ITS 67, Eqn 3.19].
        *A_ref__db = A_o__db + kHat_1 * d__meter + kHat_2 * log(d__meter);
    }
    else  // This is a trans-horizon path.
    {
        *A_ref__db = m_d * d__meter + A_ed__db;
    }

    // Set mode of propagation.
    double delta__meter = d__meter - d_l__meter;
    if (int(delta__meter) < 0)
        *propmode = MODE__LINE_OF_SIGHT;
    else
        *propmode = (int(delta__meter) == 0) ? MODE__DIFFRACTION_SINGLE_HORIZON : MODE__DIFFRACTION_DOUBLE_HORIZON;

    // Don't allow a negative loss.
    *A_ref__db = std::max(*A_ref__db, 0.0);

    return SUCCESS;
}