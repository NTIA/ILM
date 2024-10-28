/* Standard includes. */
#include <cmath>

/* Local includes. */
#include "../include/ilm.h"
#include "../include/Enums.h"

/*=============================================================================
 |
 |  Description:  Initialize area mode calculations
 |
 |        Input:  site_criteria[2]  - Siting criteria of the terminals
 |                                      + 0 : SITING_CRITERIA__MOBILE
 |                                      + 1 : SITING_CRITERIA__FIXED
 |                delta_h__meter    - Terrain irregularity parameter, in meters
 |                h__meter[2]       - Terminal structural heights, in meters
 |
 |      Outputs:  h_e__meter[2]     - Effective terminal heights, in meters
 |                d_hzn__meter[2]   - Terminal horizon distances, in meters
 |                theta_hzn[2]      - Terminal horizon angle
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
void InitializeArea(int site_criteria[2], double delta_h__meter,
    double h__meter[2], double h_e__meter[2], double d_l__meter[2],  double theta_hzn[2])
{
    for (int i = 0; i < 2; i++)
    {
        if (site_criteria[i] == SITING_CRITERIA__MOBILE)
        {
            // [RLS, A-4 & B-4]
            h_e__meter[i] = h__meter[i];
        }
        else
        {
            // [RLS, A-5 & B-5]
            double B = 10;

            if (h__meter[i] < 5.0)
            {
                // [RLS, A-6 & B-6]
                B = (B - 1) * sin(0.1 * PI * h__meter[i]) + 1;
            }

            // [RLS, A-7 & B-7]
            h_e__meter[i] = h__meter[i] + B * exp(-2.0 * h__meter[i] / delta_h__meter);
        }

        // [RLS, A-8 & B-8]
        double d_ls__meter = sqrt(2.0 * h_e__meter[i] * a_e__meter);

        // [RLS, A-9 & B-9]
        d_l__meter[i] = d_ls__meter * exp(-0.07 * sqrt(delta_h__meter / MAX(h_e__meter[i], 5)));

        // [RLS, A-10 & B-10]
        theta_hzn[i] = -(2.0 * h_e__meter[i] + 0.65 * delta_h__meter * (d_ls__meter / d_l__meter[i] - 1.0)) / d_ls__meter;
    }
}