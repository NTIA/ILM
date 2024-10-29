/**
@file

This file contains the function FindHorizons() to calculate the terminal radio
horizon angles and distances.
*/

/* Local includes. */
#include "../include/ilm.h"

/**
@brief
Compute the terminals' radio horizon angle and distance.

@param[in] pfl[]
Terrain data.

@param[in] h__meter
Terminal structural heights, in meters.

@param[out] theta_hzn
Terminal radio horizon angle, in radians.

@param[out] d_hzn__meter
Terminal radio horizon distance, in meters.

*/
void FindHorizons(
    double pfl[],
    double h__meter[2],
    double theta_hzn[2],
    double d_hzn__meter[2]
) {
    int np = int(pfl[0]);
    double xi = pfl[1];

    double d__meter = pfl[0] * pfl[1];

    /*
    Compute radials.
    Ignore radius of moon since it cancels out in the later math.
    */

    double z_tx__meter = pfl[2] + h__meter[0];
    double z_rx__meter = pfl[np + 2] + h__meter[1];

    theta_hzn[0] = (z_rx__meter - z_tx__meter) / d__meter - d__meter / (2 * a_m__meter);
    theta_hzn[1] = -(z_rx__meter - z_tx__meter) / d__meter - d__meter / (2 * a_m__meter);

    d_hzn__meter[0] = d__meter;
    d_hzn__meter[1] = d__meter;

    double d_tx__meter = 0.0;
    double d_rx__meter = d__meter;

    double theta_tx, theta_rx;

    for (int i = 1; i < np; i++)
    {
        d_tx__meter = d_tx__meter + xi;
        d_rx__meter = d_rx__meter - xi;

        theta_tx = (pfl[i + 2] - z_tx__meter) / d_tx__meter - d_tx__meter / (2 * a_m__meter);
        theta_rx = -(z_rx__meter - pfl[i + 2]) / d_rx__meter - d_rx__meter / (2 * a_m__meter);

        if (theta_tx > theta_hzn[0])
        {
            theta_hzn[0] = theta_tx;
            d_hzn__meter[0] = d_tx__meter;
        }

        if (theta_rx > theta_hzn[1])
        {
            theta_hzn[1] = theta_rx;
            d_hzn__meter[1] = d_rx__meter;
        }
    }
}