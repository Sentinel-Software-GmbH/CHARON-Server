/*
 * uds_UploadDownloadFunctionalUnit.h
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */

#ifndef UDS_UPLOADDOWNLOADFUNCTIONALUNIT_H_
#define UDS_UPLOADDOWNLOADFUNCTIONALUNIT_H_

#include <stdint.h>

uint32_t uds_UploadDownloadFunctionalUnit_RequestDownload (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_UploadDownloadFunctionalUnit_RequestUpload (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_UploadDownloadFunctionalUnit_TransferData (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_UploadDownloadFunctionalUnit_RequestTransferExit (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_UploadDownloadFunctionalUnit_RequestFileTransfer (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);


#ifdef TEST

void uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress (uint32_t newAddress);

void uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength (uint32_t newLength);

void uds_UploadDownloadFunctionalUnit_setTransferDirection (uint8_t newDirection);

void uds_UploadDownloadFunctionalUnit_setNextSequenceCounter (uint8_t newCounter);

uint32_t uds_UploadDownloadFunctionalUnit_getCurrentMemoryAddress (void);

uint32_t uds_UploadDownloadFunctionalUnit_getRemainingMemoryLength (void);

uint8_t uds_UploadDownloadFunctionalUnit_getTransferDirection (void);

uint8_t uds_UploadDownloadFunctionalUnit_getNextSequenceCounter (void);

#endif


#endif /* UDS_UPLOADDOWNLOADFUNCTIONALUNIT_H_ */
