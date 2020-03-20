/*
 * charon_negativeResponse.h
 *
 *  Created on: 03.02.2020
 *      Author: Florian Kaup
 */

#ifndef CHARON_NEGATIVERESPONSE_H_
#define CHARON_NEGATIVERESPONSE_H_

#include "charon_types.h"
#include <stdint.h>

uint32_t charon_generateNegativeResponse (uds_responseCode_t ResponseCode, uds_sid_t RequestedSid, uint8_t * transmitBuffer);


#endif /* CHARON_NEGATIVERESPONSE_H_ */
