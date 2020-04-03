/*
 * charon_negativeResponse.h
 *
 *  Created on: 03.02.2020
 *      Author: Florian Kaup
 */

#ifndef CHARON_NEGATIVERESPONSE_H_
#define CHARON_NEGATIVERESPONSE_H_

#include "Common/charon_types.h"
#include <stdint.h>

void charon_sendNegativeResponse (uds_responseCode_t ResponseCode, uds_sid_t RequestedSid);


#endif /* CHARON_NEGATIVERESPONSE_H_ */
