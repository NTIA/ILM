/**
@file

This file contains the PointToPoint() and PointToPoint_Ex() functions.
*/

/* Standard includes. */
#include <complex>

/* Local includes. */
#include "./include/ilm.h"
#include "./include/Enums.h"
#include "./include/Errors.h"

/**
@brief
The Irregular Lunar Model (ILM) Point-To-Point mode.

@param[in] h_tx__meter
Structural height of the TX, in meters.

@param[in] h_rx__meter
Structural height of the RX, in meters.

@param[in] pfl
Terrain data, in PFL format.

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
int PointToPoint(
    double h_tx__meter,
    double h_rx__meter,
    double pfl[],
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    double p,
    double *A__db,
    long *warnings
) {
    IntermediateValues interValues;

    return PointToPoint_Ex(
        h_tx__meter,
        h_rx__meter,
        pfl,
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
The Irregular Lunar Model (ILM) Point-To-Point mode.

@param[in] h_tx__meter
Structural height of the TX, in meters.

@param[in] h_rx__meter
Structural height of the RX, in meters.

@param[in] pfl
Terrain data, in PFL format.

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
int PointToPoint_Ex(
    double h_tx__meter,
    double h_rx__meter,
    double pfl[],
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    double p,
    double *A__db,
    long *warnings,
    IntermediateValues *interValues
) {
    // Terrain irregularity parameter.
    double delta_h__meter;
    // Path distance, in meters.
    double d__meter;
    // Ground impedance.
    std::complex<double> Z_g;
    // Terminal horizon angles.
    double theta_hzn[2];
    // Terminal horizon distances.
    double d_hzn__meter[2];
    // Terminal effective heights.
    double h_e__meter[2];

    // Initialize to no warnings.
    *warnings = NO_WARNINGS;

    // Initial input validation check.
    // Some validation occurs later in calculations.
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

    interValues->d__km = (pfl[0] * pfl[1]) / 1000.0;

    // Number of points in the pfl.
    int np = int(pfl[0]);

    p /= 100.0;

    /* Compute the average path height, ignoring first and last 10%. */
    // 10% of np
    int p10 = int(0.1 * np);
    // Height of the system above mean sea level.
    double h_sys__meter = 0.0;

    for (int i = p10; i <= np - p10; i++)
        h_sys__meter += pfl[i + 2];

    h_sys__meter = h_sys__meter / (np - 2 * p10 + 1);

    InitializePointToPoint(
        f__mhz,
        pol,
        epsilon,
        sigma,
        &Z_g
    );

    double h__meter[2] = { h_tx__meter, h_rx__meter };
    QuickPfl(
        pfl,
        h__meter,
        theta_hzn,
        d_hzn__meter,
        h_e__meter,
        &delta_h__meter,
        &d__meter
    );

    // Reference attenuation, in dB.
    double A_ref__db = 0.0;
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
};
