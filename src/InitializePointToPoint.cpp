#include "../include/ilm.h"
#include "../include/Enums.h"

/*=============================================================================
 |
 |  Description:  Initialize parameters for point-to-point mode
 |
 |        Input:  f__mhz            - Frequency, in MHz
 |                pol               - Polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                epsilon           - Relative permittivity
 |                sigma             - Conductivity
 |
 |      Outputs:  Z_g               - Complex ground impedance
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
void InitializePointToPoint(double f__mhz, int pol, double epsilon, double sigma, complex<double> *Z_g)
{
    // complex relative permittivity
    complex<double> ep_r = complex<double>(epsilon, 18000 * sigma / f__mhz);

    // [RLS, A-3]
    *Z_g = sqrt(ep_r - 1.0);                        // ground impedance (horizontal polarization)
    if (pol == POLARIZATION__VERTICAL)              // adjust for vertical polarization
        *Z_g = *Z_g / ep_r;
}