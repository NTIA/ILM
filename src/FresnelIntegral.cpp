/**
@file

This file contains the FresnelIntegral() function.
*/

/* Standard includes. */
#include <cmath>

/* Local includes. */
#include "../include/ilm.h"

/**
@brief
Approximation to ideal knife edge diffraction loss.

@param[in] v2
v^2 parameter.

@return A(v, 0)
Loss, in dB.

*/
double FresnelIntegral(
    double v2
) {
    // Note: v2  is v^2, so 5.76 is actually comparing v to 2.4.
    if (v2 < 5.76)
        // [TN101v2, Eqn III.24b] and [ERL 79-ITS 67, Eqn 3.27a & 3.27b].
        return 6.02 + 9.11 * sqrt(v2) - 1.27 * v2;
    else
        // [TN101v2, Eqn III.24c] and [ERL 79-ITS 67, Eqn 3.27a & 3.27b].
        return 12.953 + 10 * log10(v2);
}