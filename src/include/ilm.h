#pragma once
/**
@file

ITU-R SG3 Irregular Lunar Model (ILM).

The ILM is a modification of the NTIA/ITS Irregular Terrain Model (ITM) to be
applicable to the surface of the Earth's moon.
*/

// ILM.dll header guard.
#ifndef ILM_H
#define ILM_H

/* Standard includes. */
#include <complex>

/**
@brief
ILM.dll version number.
*/
#define VERSION R"(0.0.0)"
/**
@brief
ILM.dll compilation date and time.
*/
#define COMPILE_TIME __DATE__ " " __TIME__

/* Usings. */
using namespace std;

/* Utility functions. */
/**
@brief
Maximum of x and y.
*/
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/**
@brief
Minimum of x and y.
*/
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/**
@brief
Dim of x and y.
*/
#define DIM(x, y) (((x) > (y)) ? (x - y) : (0))

/* Constants. */
/**
@brief
The ratio of a circle's circumference to its diameter.
*/
#define PI 3.1415926535897932384

/**
@brief
The radius of Earth's moon in meters.
*/
#define a_m__meter 1737400

/**
@brief
Structure to hold intermediate values for debugging output.
*/
struct IntermediateValues
{
    /**
    Terminal horizon angles.
    */
    double theta_hzn[2];

    /**
    Terminal horizon distances, in meters.
    */
    double d_hzn__meter[2];

    /**
    Terminal effective heights, in meters.
    */
    double h_e__meter[2];

    /**
    Terrain irregularity parameter, in meters.
    */
    double delta_h__meter;

    /**
    Reference attenuation, in dB.
    */
    double A_ref__db;

    /**
    Free space basic transmission loss, in dB.
    */
    double A_fs__db;

    /**
    Path distance, in km.
    */
    double d__km;

    /**
    Mode of propagation value.
    */
    int mode;
};

/* DLL export/import. */
/**
_WIN32 indicates compilation on a Windows OS.
Define _WIN32 in:
Visual Studio > Projects > ILM_DLL Properties > C/C++ > Preprocessor
> Preprocessor Definitions.
*/
#ifdef _WIN32

/**
ILM_DLL_EXPORT indicates if the library functions should be exported or
imported.
Only define ILM_DLL_EXPORT to export the library functions for creating
ILM.dll. Do not define ILM_DLL_EXPORT in projects that import ILM.dll and the
library functions will be imported.

Define ILM_DLL_EXPORT in:
Visual Studio > Project > MKE Properties > C/C++ > Preprocessor
> Preprocessor Definitions.
*/
#ifdef ILM_DLL_EXPORT
#define ILM_API extern "C" __declspec(dllexport)
#else
#define ILM_API extern "C" __declspec(dllimport)
#endif  // ILM_DLL_EXPORT
#else
// Compiling with a non-Windows OS.
#define ILM_API
#endif  // _WIN32

/* Prototypes. */

ILM_API char const *version();

ILM_API char const *compile_time();

/* Primary ILM library functions. */

ILM_API int PointToPoint(
    double h_tx__meter,
    double h_rx__meter,
    double pfl[],
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    double p,
    double* A__db,
    long* warnings
);

ILM_API int PointToPoint_Ex(
    double h_tx__meter,
    double h_rx__meter,
    double pfl[],
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    double p,
    double* A__db,
    long* warnings,
    IntermediateValues* interValues
);

ILM_API int Area(
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
    double* A__db,
    long* warnings
);

ILM_API int Area_Ex(
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
    double* A__db,
    long* warnings,
    IntermediateValues* interValues
);

/* ILM Helper Functions. */

ILM_API double ComputeDeltaH(
    double pfl[],
    double d_start__meter,
    double d_end__meter
);

ILM_API double DiffractionLoss(
    double d__meter,
    double d_hzn__meter[2],
    double h_e__meter[2],
    complex<double> Z_g,
    double delta_h__meter,
    double h__meter[2],
    double theta_los,
    double f__mhz
);

ILM_API void FindHorizons(
    double pfl[],
    double h__meter[2],
    double theta_hzn[2],
    double d_hzn__meter[2]
);

ILM_API double FreeSpaceLoss(
    double d__meter,
    double f__mhz
);

ILM_API double FresnelIntegral(
    double v2
);

ILM_API double HeightFunction(
    double x__meter,
    double K
);

ILM_API void InitializeArea(
    int site_criteria[2],
    double delta_h__meter,
    double h__meter[2],
    double h_e__meter[2],
    double d_hzn__meter[2],
    double theta_hzn[2]
);

ILM_API void InitializePointToPoint(
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    complex<double> *Z_g
);

ILM_API double InverseComplementaryCumulativeDistributionFunction(
    double q
);

ILM_API double KnifeEdgeDiffraction(
    double d__meter,
    double f__mhz,
    double theta_los,
    double d_hzn__meter[2]
);

ILM_API void LinearLeastSquaresFit(
    double pfl[],
    double d_start,
    double d_end,
    double *fit_y1,
    double *fit_y2
);

ILM_API double LineOfSightLoss(
    double d__meter,
    double h_e__meter[2],
    complex<double> Z_g,
    double delta_h__meter,
    double M_d,
    double A_d0,
    double d_sML__meter,
    double f__mhz
);

ILM_API int LongleyRice(
    double theta_hzn[2],
    double f__mhz,
    complex<double> Z_g,
    double d_hzn__meter[2],
    double h_e__meter[2],
    double delta_h__meter,
    double h__meter[2],
    double d__meter,
    double* A_ref__db,
    long* warnings,
    int* propmode
);

ILM_API void QuickPfl(
    double pfl[],
    double h__meter[2],
    double theta_hzn[2],
    double d_hzn__meter[2],
    double h_e__meter[2],
    double *delta_h__meter,
    double *d__meter
);

ILM_API double SigmaHFunction(
    double delta_h__meter
);

ILM_API double SmoothMoonDiffraction(
    double d__meter,
    double f__mhz,
    double theta_los,
    double d_hzn__meter[2],
    double h_e__meter[2],
    complex<double> Z_g
);

ILM_API double TerrainRoughness(
    double d__meter,
    double delta_h__meter
);

ILM_API int ValidateInputs(
    double h_tx__meter,
    double h_rx__meter,
    double p,
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    long *warnings
);

ILM_API double Variability(
    double p,
    double delta_h__meter,
    double f__mhz,
    double d__meter,
    double A_ref__db
);

#endif  // ILM_H
