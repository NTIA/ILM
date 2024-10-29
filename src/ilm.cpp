/**
@file

This file contains functions to return the version and compile time of ILM.dll.
*/

/* Local includes. */
#include "./include/ilm.h"

/* Implementations. */

/**
Return the version of ILM.dll.

INPUT
    None

OUTPUT
    Returns a pointer to the VERSION character string.

SUBROUTINES
    None

*/
char const *version() {
    return VERSION;
};

/**
Return the compile time of ILM.dll.

INPUT
    None

OUTPUT
    Returns a pointer to the COMPILE_TIME character string.

SUBROUTINES
    None

*/
char const *compile_time() {
    return COMPILE_TIME;
};
