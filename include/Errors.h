#pragma once
/**
@file

Error codes for the ILM.
*/

/**
Successful return.
*/
#define SUCCESS 0

/**
No warnings.
*/
#define NO_WARNINGS 0

/**
Successfull return with warnings.
*/
#define SUCCESS_WITH_WARNINGS 1

/**
TX terminal height is out of range.
*/
#define ERROR__TX_TERMINAL_HEIGHT 1000

/**
RX terminal height is out of range.
*/
#define ERROR__RX_TERMINAL_HEIGHT 1001

/**
Location percentage is out of range.
*/
#define ERROR__INVALID_PERCENTAGE 1002

/**
Refractivity is out of range.
*/
#define ERROR__REFRACTIVITY 1003

/**
Frequency is out of range.
*/
#define ERROR__FREQUENCY 1004

/**
Invalid value for polarization.
*/
#define ERROR__POLARIZATION 1005

/**
Epsilon is out of range.
*/
#define ERROR__EPSILON 1006

/**
Sigma is out of range.
*/
#define ERROR__SIGMA 1007

/**
The imaginary portion of the complex impedance is larger than the real portion.
*/
#define ERROR__GROUND_IMPEDANCE 1008

/**
Path distance is out of range.
*/
#define ERROR__PATH_DISTANCE 1009

/**
Delta H (terrain irregularity parameter) is out of range.
*/
#define ERROR__DELTA_H 1010

/**
Invalid value for TX siting criteria.
*/
#define ERROR__TX_SITING_CRITERIA 1011

/**
Invalid value for RX siting criteria.
*/
#define ERROR__RX_SITING_CRITERIA 1012
