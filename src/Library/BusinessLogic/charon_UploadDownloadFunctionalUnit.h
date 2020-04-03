/*
 * charon_UploadDownloadFunctionalUnit.h
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */

#ifndef CHARON_UPLOADDOWNLOADFUNCTIONALUNIT_H_
#define CHARON_UPLOADDOWNLOADFUNCTIONALUNIT_H_

#include <stdint.h>
#include "Common/charon_types.h"

uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestDownload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestUpload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t charon_UploadDownloadFunctionalUnit_TransferData (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestTransferExit (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestFileTransfer (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);


#ifdef TEST

void charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress (uint32_t newAddress);

void charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength (uint32_t newLength);

void charon_UploadDownloadFunctionalUnit_setTransferDirection (uint8_t newDirection);

void charon_UploadDownloadFunctionalUnit_setNextSequenceCounter (uint8_t newCounter);

uint32_t charon_UploadDownloadFunctionalUnit_getCurrentMemoryAddress (void);

uint32_t charon_UploadDownloadFunctionalUnit_getRemainingMemoryLength (void);

uint8_t charon_UploadDownloadFunctionalUnit_getTransferDirection (void);

uint8_t charon_UploadDownloadFunctionalUnit_getNextSequenceCounter (void);

#endif


#endif /* CHARON_UPLOADDOWNLOADFUNCTIONALUNIT_H_ */
