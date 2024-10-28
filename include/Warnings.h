#pragma once
/**
@file

Warning flags for the ILM.
*/

/**
TX terminal height is near its limits.
*/
#define WARN__TX_TERMINAL_HEIGHT 0x0001

/**
RX terminal height is near its limits.
*/
#define WARN__RX_TERMINAL_HEIGHT 0x0002

/**
Frequency is near its limits.
*/
#define WARN__FREQUENCY 0x0004

/**
Path distance is near its upper limit.
*/
#define WARN__PATH_DISTANCE_TOO_BIG_1 0x0008

/**
Path distance is large - care must be taken with result.
*/
#define WARN__PATH_DISTANCE_TOO_BIG_2 0x0010

/**
Path distance is near its lower limit.
*/
#define WARN__PATH_DISTANCE_TOO_SMALL_1 0x0020

/**
Path distance is small - care must be taken with result.
*/
#define WARN__PATH_DISTANCE_TOO_SMALL_2 0x0040

/**
TX horizon angle is large - small angle approximations could break down.
*/
#define WARN__TX_HORIZON_ANGLE 0x0080

/**
RX horizon angle is large - small angel approximations could break down.
*/
#define WARN__RX_HORIZON_ANGLE 0x0100

/**
TX horizon distance is less than 1/10 of the smooth earth horizon distance.
*/
#define WARN__TX_HORIZON_DISTANCE_1 0x0200

/**
RX horizon distance is less than 1/10 of the smooth earth horizon distance.
*/
#define WARN__RX_HORIZON_DISTANCE_1 0x0400

/**
TX horizon distance is greater than 3 times the smooth earth horizon distance.
*/
#define WARN__TX_HORIZON_DISTANCE_2 0x0800

/**
RX horizon distance is greater than 3 times the smooth earth horizon distance.
*/
#define WARN__RX_HORIZON_DISTANCE_2 0x1000
