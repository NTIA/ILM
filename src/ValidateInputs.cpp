#include "../include/ilm.h"
#include "../include/Enums.h"
#include "../include/Errors.h"
#include "../include/Warnings.h"

/*=============================================================================
 |
 |  Description:  Perform input parameter validation.  This function only
 |                applies to the set of variables common to both ILM
 |                point-to-point mode and area mode.
 |
 |        Input:  h_tx__meter    - Structural height of the TX, in meters
 |				  h_rx__meter    - Structural height of the RX, in meters
 |                p              - Location percentage, 0 < p < 100
 |                f__mhz         - Frequency, in MHz
 |                pol            - Polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                epsilon        - Relative permittivity
 |                sigma          - Conductivity
 |
 |      Outputs:  warnings       - Warning messages
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
int ValidateInputs(double h_tx__meter, double h_rx__meter, double p, 
    double f__mhz, int pol, double epsilon, double sigma, long *warnings)
{
    if (h_tx__meter < 1.0 || h_tx__meter > 1000.0)
        *warnings |= WARN__TX_TERMINAL_HEIGHT;

    if (h_tx__meter < 0.5 || h_tx__meter > 3000.0)
        return ERROR__TX_TERMINAL_HEIGHT;

    if (h_rx__meter < 1.0 || h_rx__meter > 1000.0)
        *warnings |= WARN__RX_TERMINAL_HEIGHT;

    if (h_rx__meter < 0.5 || h_rx__meter > 3000.0)
        return ERROR__RX_TERMINAL_HEIGHT;

    if (f__mhz < 40.0 || f__mhz > 10000.0)
        *warnings |= WARN__FREQUENCY;

    if (f__mhz < 20 || f__mhz > 20000)
        return ERROR__FREQUENCY;

    if (pol != POLARIZATION__HORIZONTAL &&
        pol != POLARIZATION__VERTICAL)
        return ERROR__POLARIZATION;

    if (epsilon < 1)
        return ERROR__EPSILON;

    if (sigma <= 0)
        return ERROR__SIGMA;

    if (p <= 0 || p >= 100)
        return ERROR__INVALID_PERCENTAGE;

    return SUCCESS;
}
