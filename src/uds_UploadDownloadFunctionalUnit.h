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

#endif /* UDS_UPLOADDOWNLOADFUNCTIONALUNIT_H_ */
