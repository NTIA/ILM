/**
@file

This file contains the QuickPfl() function.
*/

/* Standard includes. */
#include <cmath>

/* Local includes. */
#include "./include/ilm.h"

/**
@brief
Extract parameters from the terrain pfl.

@param[in] pfl
Terrain data in pfl format.

@param[in] h__meter
Terminal structural heights, in meters.

@param[out] theta_hzn
Terminal horizon angles.

@param[out] d_hzn__meter
Terminal horizon distances, in meters.

@param[out] h_e__meter
Effective terminal heights, in meters.

@param[out] delta_h__meter
Terrain irregularity parameter.

@param[out] d__meter
Path distance, in meters.

*/
void QuickPfl(
    double pfl[],
    double h__meter[2],
    double theta_hzn[2],
    double d_hzn__meter[2],
    double h_e__meter[2],
    double *delta_h__meter,
    double *d__meter
) {
    double fit_tx;
    double fit_rx;
    double q;
    double d_start__meter;
    double d_end__meter;

    *d__meter = pfl[0] * pfl[1];

    int np = int(pfl[0]);

    FindHorizons(
        pfl,
        h__meter,
        theta_hzn,
        d_hzn__meter
    );

    /**
    "In our own work we have sometimes said that consideration of terrain
    elevations should begin at a point about 15 times the tower height."
    - [Hufford, 1982] Page 25
    */

    // Take lesser: 10% of horizon distance or 15x terminal height.
    d_start__meter = MIN(15.0 * h__meter[0], 0.1 * d_hzn__meter[0]);

    // Same, but measured from the far end of the link.
    d_end__meter = *d__meter - MIN(15.0 * h__meter[1], 0.1 * d_hzn__meter[1]);

    *delta_h__meter = ComputeDeltaH(
        pfl,
        d_start__meter,
        d_end__meter
    );

    if (d_hzn__meter[0] + d_hzn__meter[1] > 1.5 * *d__meter)
    {
        /*
        The combined horizon distance is at least 50% larger than the total
        path distance so we are well within the line-of-sight range.
        */

        LinearLeastSquaresFit(
            pfl,
            d_start__meter,
            d_end__meter,
            &fit_tx,
            &fit_rx
        );

        h_e__meter[0] = h__meter[0] + fdim(pfl[2], fit_tx);
        h_e__meter[1] = h__meter[1] + fdim(pfl[np + 2], fit_rx);

        for (int i = 0; i < 2; i++)
            d_hzn__meter[i] = sqrt(2.0 * h_e__meter[i] * a_m__meter) * exp(-0.07 * sqrt(*delta_h__meter / MAX(h_e__meter[i], 5.0)));

        double combined_horizons__meter = d_hzn__meter[0] + d_hzn__meter[1];
        if (combined_horizons__meter <= *d__meter)
        {
            q = pow(*d__meter / combined_horizons__meter, 2);

            for (int i = 0; i < 2; i++)
            {
                h_e__meter[i] = h_e__meter[i] * q;
                d_hzn__meter[i] = sqrt(2.0 * h_e__meter[i] * a_m__meter) * exp(-0.07 * sqrt(*delta_h__meter / MAX(h_e__meter[i], 5.0)));
            }
        }

        for (int i = 0; i < 2; i++)
        {
            q = sqrt(2.0 * h_e__meter[i] * a_m__meter);
            theta_hzn[i] = (0.65 * *delta_h__meter * (q / d_hzn__meter[i] - 1.0) - 2.0 * h_e__meter[i]) / q;
        }
    }
    else
    {
        double dummy = 0.0;

        LinearLeastSquaresFit(
            pfl,
            d_start__meter,
            0.9 * d_hzn__meter[0],
            &fit_tx,
            &dummy
        );
        h_e__meter[0] = h__meter[0] + fdim(pfl[2], fit_tx);

        LinearLeastSquaresFit(
            pfl,
            *d__meter - 0.9 * d_hzn__meter[1],
            d_end__meter,
            &dummy, &fit_rx
        );
        h_e__meter[1] = h__meter[1] + fdim(pfl[np + 2], fit_rx);
    }
}