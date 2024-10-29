/**
@file

This file contains the function ComputeDeltaH() to calculate the terrain
irregularity parameter, delta_h, in meters.
*/

/* Standard includes. */
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>

/* Local includes. */
#include "./include/ilm.h"

/**
* @brief
Compute the terrain irregularity parameter, delta_h.

@param[in] pfl[]
Terrain data.

@param[in] d_start__meter
Distance into the terrain profile to start considering data, in meters.

@param[in] d_end__meter
Distance into the terrain profile to end considering data, in meters.

@return delta_h__meter
Terrain irregularity parameter, in meters.
*/
double ComputeDeltaH(
    double pfl[],
    double d_start__meter,
    double d_end__meter
) {
    // Temp pfl data array.
    double s[247] = { 0 };

    int np = (int)pfl[0];

    // index to start considering terrain points.
    double x_start = d_start__meter / pfl[1];

    // index to stop considering terrain points.
    double x_end = d_end__meter / pfl[1];

    // If there are less than 2 terrain points, return delta_h = 0.
    if (x_end - x_start < 2.0)
        return 0;

    // 10% index.
    int p10 = (int)(0.1 * (x_end - x_start + 8.0));
    p10 = std::min(std::max(4, p10), 25);

    int n = 10 * p10 - 5;
    // 90% index.
    int p90 = n - p10;

    double np_s = n - 1;
    s[0] = np_s;
    s[1] = 1.0;

    x_end = (x_end - x_start) / np_s;
    int i = (int)x_start;
    x_start -= float(i + 1.0);

    for (int j = 0; j < n; j++)
    {
        while (x_start > 0.0 && (i + 1) < np)
        {
            x_start--;
            i++;
        }

        s[j + 2] = pfl[i + 3] + (pfl[i + 3] - pfl[i + 2]) * x_start;

        x_start += x_end;
    }

    double fit_y1;
    double fit_y2;
    LinearLeastSquaresFit(
        s,
        0.0,
        np_s,
        &fit_y1,
        &fit_y2
    );
    fit_y2 = (fit_y2 - fit_y1) / np_s;

    std::vector<double> diffs;
    
    // Compute the difference between fitted line and actual data.
    for (int j = 0; j < n; j++)
    {
        diffs.push_back(s[j + 2] - fit_y1);
        fit_y1 += fit_y2;
    }

    std::nth_element(
        diffs.begin(),
        diffs.begin() + p10 - 1,
        diffs.end(),
        std::greater<double>()
    );
    double q10 = diffs[p10 - 1];

    std::nth_element(
        diffs.begin(),
        diffs.begin() + p90,
        diffs.end(),
        std::greater<double>()
    );
    double q90 = diffs[p90];

    double delta_h_d__meter = q10 - q90;

    // [ERL 79-ITS 67, Eqn 3], inverted.
    double delta_h__meter = delta_h_d__meter / (1.0 - 0.8 * exp(-(d_end__meter - d_start__meter) / 50.0E3));

    return delta_h__meter;
}