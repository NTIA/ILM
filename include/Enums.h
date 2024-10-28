#pragma once
/**
@file

Enums for the ILM.
*/

/**
Horizontal polarization indicator.
*/
#define POLARIZATION__HORIZONTAL 0

/**
Vertical polarization indicator.
*/
#define POLARIZATION__VERTICAL 1

/**
Mobile siting criteria.
*/
#define SITING_CRITERIA__MOBILE 0

/**
Fixed siting criteria.
*/
#define SITING_CRITERIA__FIXED 1

// List of valid modes of propagation

/**
Propagation mode is not set.
*/
#define MODE__NOT_SET 0

/**
Propagation mode is line of sight.
*/
#define MODE__LINE_OF_SIGHT 10

/**
Propagation mode is diffraction, single horizon.
*/
#define MODE__DIFFRACTION_SINGLE_HORIZON 20

/**
Propagation mode is diffraction, double horizon.
*/
#define MODE__DIFFRACTION_DOUBLE_HORIZON 21
