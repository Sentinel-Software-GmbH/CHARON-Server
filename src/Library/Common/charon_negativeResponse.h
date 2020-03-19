/*
 * uds_negativeResponse.h
 *
 *  Created on: 03.02.2020
 *      Author: Florian Kaup
 */

#ifndef SRC_UDS_NEGATIVERESPONSE_H_
#define SRC_UDS_NEGATIVERESPONSE_H_

#include "charon_types.h"
#include <stdint.h>

uint32_t uds_generateNegativeResponse (uds_responseCode_t ResponseCode, uds_sid_t RequestedSid, uint8_t * transmitBuffer);


#endif /* SRC_UDS_NEGATIVERESPONSE_H_ */
