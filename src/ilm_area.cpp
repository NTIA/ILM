/**
@file

This file contains the Area() and Area_Ex() functions.
*/

/* Standard includes. */
#include <complex>

/* Local includes. */
#include "./include/ilm.h"
#include "./include/Enums.h"
#include "./include/Errors.h"

/**
@brief
The Irregular Lunar Model (ILM) Point-to-Area mode.

@param[in] h_tx__meter
Structural height of the TX, in meters.

@param[in] h_rx__meter
Structural height of the RX, in meters.

@param[in] tx_site_criteria
Siting criteria of the TX.

@param[in] rx_site_criteria
Siting criteria of the RX.
Either:
    0: SITING_CRITERIA__MOBILE
    1: SITING_CRITERIA__FIXED

@param[in] d__km
Path distance, in km.

@param[in] delta_h__meter
Terrain irregularity parameter.

@param[in] f__mhz
Frequency, in MHz.

@param[in] pol
Polarization.
Either:
    0: POLARIZATION__HORIZONTAL
    1: POLARIZATION__VERTICAL

@param[in] epsilon
Relative permittivity.

@param[in] sigma
Conductivity.

@param[in] p
Location percentage, 0 < p < 100.

@param[out] A__db
Basic transmission loss, in dB.

@param[out] warnings
Warning flags.

@return error
Error code.

*/
int Area(
    double h_tx__meter,
    double h_rx__meter,
    int tx_site_criteria,
    int rx_site_criteria,
    double d__km,
    double delta_h__meter,
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    double p,
    double *A__db,
    long *warnings
) {
    IntermediateValues interValues;

    return Area_Ex(
        h_tx__meter,
        h_rx__meter,
        tx_site_criteria,
        rx_site_criteria,
        d__km,
        delta_h__meter,
        f__mhz,
        pol,
        epsilon,
        sigma,
        p,
        A__db,
        warnings,
        &interValues
    );
}

/**
@brief
The Irregular Lunar Model (ILM) Point-to-Area mode.

@param[in] h_tx__meter
Structural height of the TX, in meters.

@param[in] h_rx__meter
Structural height of the RX, in meters.

@param[in] tx_site_criteria
Siting criteria of the TX.
Either:
    0: SITING_CRITERIA__MOBILE
    1: SITING_CRITERIA__FIXED

@param[in] rx_site_criteria
Siting criteria of the RX.
Either:
    0: SITING_CRITERIA__MOBILE
    1: SITING_CRITERIA__FIXED

@param[in] d__km
Path distance, in km.

@param[in] delta_h__meter
Terrain irregularity parameter.

@param[in] f__mhz
Frequency, in MHz.

@param[in] pol
Polarization.
Either:
    0: POLARIZATION__HORIZONTAL
    1: POLARIZATION__VERTICAL

@param[in] epsilon
Relative permittivity.

@param[in] sigma
Conductivity.

@param[in] p
Location percentage, 0 < p < 100.

@param[out] A__db
Basic transmission loss, in dB.

@param[out] warnings
Warning flags.

@param[out] interValues
Struct of intermediate values.

@return error
Error code.

*/
int Area_Ex(
    double h_tx__meter,
    double h_rx__meter,
    int tx_site_criteria,
    int rx_site_criteria,
    double d__km,
    double delta_h__meter,
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    double p,
    double *A__db,
    long *warnings,
    IntermediateValues *interValues
) {
    *warnings = NO_WARNINGS;

    // Initial input validation check.
    // Some validation occurs later in calculations as well.
    int rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        p,
        f__mhz,
        pol,
        epsilon,
        sigma,
        warnings
    );
    if (rtn != SUCCESS)
        return rtn;

    // Switch from percentages to ratios.
    p /= 100;

    // Additional area mode parameter validation checks.
    if (d__km <= 0)
        return ERROR__PATH_DISTANCE;
    if (delta_h__meter < 0)
        return ERROR__DELTA_H;
    if (tx_site_criteria != SITING_CRITERIA__MOBILE &&
        tx_site_criteria != SITING_CRITERIA__FIXED)
        return ERROR__TX_SITING_CRITERIA;
    if (rx_site_criteria != SITING_CRITERIA__MOBILE &&
        rx_site_criteria != SITING_CRITERIA__FIXED)
        return ERROR__RX_SITING_CRITERIA;

    int site_criteria[2] = { tx_site_criteria, rx_site_criteria };
    double h__meter[2] = { h_tx__meter, h_rx__meter };
    interValues->d__km = d__km;

    double theta_hzn[2];
    double d_hzn__meter[2];
    double h_e__meter[2];
    complex<double> Z_g;
    double A_ref__db = 0.0;

    InitializePointToPoint(
        f__mhz,
        pol,
        epsilon,
        sigma,
        &Z_g
    );

    InitializeArea(
        site_criteria,
        delta_h__meter,
        h__meter,
        h_e__meter,
        d_hzn__meter,
        theta_hzn
    );

    double d__meter = d__km * 1000.0;
    int propmode = MODE__NOT_SET;
    rtn = LongleyRice(
        theta_hzn,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        delta_h__meter,
        h__meter,
        d__meter,
        &A_ref__db,
        warnings,
        &propmode
    );
    if (rtn != SUCCESS)
        return rtn;

    double A_fs__db = FreeSpaceLoss(
        d__meter,
        f__mhz
    );

    *A__db = A_fs__db
        + Variability(
            p,
            delta_h__meter,
            f__mhz,
            d__meter,
            A_ref__db
        );

    // Save intermediate values.
    interValues->A_ref__db = A_ref__db;
    interValues->A_fs__db = A_fs__db;
    interValues->delta_h__meter = delta_h__meter;
    interValues->d_hzn__meter[0] = d_hzn__meter[0];
    interValues->d_hzn__meter[1] = d_hzn__meter[1];
    interValues->h_e__meter[0] = h_e__meter[0];
    interValues->h_e__meter[1] = h_e__meter[1];
    interValues->theta_hzn[0] = theta_hzn[0];
    interValues->theta_hzn[1] = theta_hzn[1];
    interValues->mode = propmode;

    if (*warnings != NO_WARNINGS)
        return SUCCESS_WITH_WARNINGS;

    return SUCCESS;
}