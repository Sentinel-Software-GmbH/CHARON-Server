/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * @addtogroup CharonUDS_Server
 * @{
 * @addtogroup showcaseDemoPort showcase demo
 * @{
 * @file charon_interface_debug.c
 * Implementation of debug interface
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 *
 *******************************************************************************/

/* Includes ******************************************************************/

#include "charon_interface_debug.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

/** @brief Function uses snprintf() to log Charon Info messages.
 * 
 * @param x String to log.
 */
void CHARON_INFO (char *x, ...)
{
    /* Declare a va_list type variable */
    va_list myargs;
    char catArray[512];

    /* Initialise the va_list variable with the ... after fmt */
    va_start(myargs, x);

    snprintf(catArray, 512, "[CHARON_INFO] %s\n\r", x);

    /* Forward the '...' to vprintf */
    vprintf(catArray, myargs);
    fflush(stdout);

    /* Clean up the va_list */
    va_end(myargs);
}

/** @brief Function uses snprintf() to log Charon Info messages.
 * 
 * @param x String to log.
 */
void CHARON_WARNING (char *x, ...)
{
    /* Declare a va_list type variable */
    va_list myargs;
    char catArray[512];

    /* Initialise the va_list variable with the ... after fmt */
    va_start(myargs, x);

    snprintf(catArray, 512, "[CHARON_WARNING] %s\n\r", x);

    /* Forward the '...' to vprintf */
    vprintf(catArray, myargs);
    fflush(stdout);

    /* Clean up the va_list */
    va_end(myargs);
}

/** @brief Function uses snprintf() to log Charon Info messages.
 * 
 * @param x String to log.
 */
void CHARON_ERROR (char *x, ...)
{
    /* Declare a va_list type variable */
    va_list myargs;
    char catArray[512];

    /* Initialise the va_list variable with the ... after fmt */
    va_start(myargs, x);

    snprintf(catArray, 512, "[CHARON_ERROR] %s\n\r", x);

    /* Forward the '...' to vprintf */
    vprintf(catArray, myargs);
    fflush(stdout);

    /* Clean up the va_list */
    va_end(myargs);
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
