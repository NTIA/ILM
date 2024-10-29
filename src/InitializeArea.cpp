/**
@file

This file contains the InitializeArea() function.
*/

/* Standard includes. */
#define _USE_MATH_DEFINES
#include <cmath>
#include <utility>

/* Local includes. */
#include "./include/Enums.h"
#include "./include/ilm.h"


/**
@brief
Initialize area mode calculations.

@param[in] site_criteria
Siting criteria of the terminals
    0: SITING_CRITERIA__MOBILE
    1: SITING_CRITERIA__FIXED

@param[in] delta_h__meter
Terrain irregularity parameter, in meters.

@param[in] h__meter
Terminal structural heights, in meters.

@param[out] h_e__meter
Effective terminal heights, in meters.

@param[out] d_l__meter
Terminal horizon distances, in meters.

@param[out] theta_hzn
Terminal horizon angle.

*/
void InitializeArea(
    int site_criteria[2],
    double delta_h__meter,
    double h__meter[2],
    double h_e__meter[2],
    double d_l__meter[2],
    double theta_hzn[2]
) {
    for (int i = 0; i < 2; i++)
    {
        if (site_criteria[i] == SITING_CRITERIA__MOBILE)
        {
            // [RLS, A-4 & B-4].
            h_e__meter[i] = h__meter[i];
        }
        else
        {
            // [RLS, A-5 & B-5].
            double B = 10.0;

            if (h__meter[i] < 5.0)
            {
                // [RLS, A-6 & B-6].
                B = (B - 1.0) * sin(0.1 * M_PI * h__meter[i]) + 1.0;
            }

            // [RLS, A-7 & B-7].
            h_e__meter[i] = h__meter[i] + B * exp(-2.0 * h__meter[i] / delta_h__meter);
        }

        // [RLS, A-8 & B-8].
        double d_ls__meter = sqrt(2.0 * h_e__meter[i] * a_m__meter);

        // [RLS, A-9 & B-9].
        d_l__meter[i] = d_ls__meter * exp(-0.07 * sqrt(delta_h__meter / std::max(h_e__meter[i], 5.0)));

        // [RLS, A-10 & B-10].
        theta_hzn[i] = -(2.0 * h_e__meter[i] + 0.65 * delta_h__meter * (d_ls__meter / d_l__meter[i] - 1.0)) / d_ls__meter;
    }
}