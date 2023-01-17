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
 */
/**
 * @addtogroup CharonUDS_Server
 * @{
 * @addtogroup Common Common
 * @{
 * @file charon_negativeResponse.h
 * This Module handles the Transfer of the negative Response message.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_NEGATIVERESPONSE_H_
#define CHARON_NEGATIVERESPONSE_H_

/* Includes ******************************************************************/
#include "charon_types.h"
#include <stdint.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/** @brief The function is used to create a negative response of client request.
 * 
 * @param ResponseCode @ref uds_responseCode_t.
 * @param RequestedSid @ref uds_sid_t.
 */
void charon_sendNegativeResponse (uds_responseCode_t ResponseCode, uds_sid_t RequestedSid);

#endif /* CHARON_NEGATIVERESPONSE_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
