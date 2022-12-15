/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 René Mutscheller
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
 */
/**
 * @addtogroup CharonUDS_Server
 * @{
 * @defgroup Common Common
 * @{
 * @file charon_crc16.h
 * This file defines the functions charon_crc16_init(), charon_crc16_update() and charon_crc16_finalize().
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_CRC16_H_
#define CHARON_CRC16_H_

/* Includes ******************************************************************/
#include <stdlib.h>
#include <stdint.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 16 bits.
 */
typedef uint_fast16_t crc16_t;


/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param[in] data     The data word to be reflected.
 * \param[in] data_len The width of \a data expressed in number of bits.
 * \return             The reflected data.
 */
crc16_t charon_crc16_reflect(crc16_t data, size_t data_len);


/**
 * Calculate the initial crc value.
 *
 * \return      	    The initial crc value.
 */
static inline crc16_t charon_crc16_init(void)
{
    return 0x0000;
}


/**
 * Update the crc value with new data.
 *
 * \param[in] crc      The current crc value.
 * \param[in] data     Pointer to a buffer of \a data_len bytes.
 * \param[in] data_len Number of bytes in the \a data buffer.
 * \return             The updated crc value.
 */
crc16_t charon_crc16_update(crc16_t crc, const void *data, size_t data_len);


/**
 * Calculate the final crc value.
 *
 * \param[in] crc       The current crc value.
 * \return              The final crc value.
 */
static inline crc16_t charon_crc16_finalize(crc16_t crc)
{
    return charon_crc16_reflect(crc, 16);
}

#endif /* CHARON_CRC16_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
