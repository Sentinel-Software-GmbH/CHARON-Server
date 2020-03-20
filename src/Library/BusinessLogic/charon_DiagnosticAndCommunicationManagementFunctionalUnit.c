/*
 * charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
 *
 *  Created on: 30.01.2020
 *      Author: Florian Kaup
 */


#include "charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "charon_negativeResponse.h"
#include "charon_types.h"

typedef enum {
    SessionType_IsoSaeReserved,
    SessionType_defaultSession,
    SessionType_ProgrammingSession,
    SessionType_extendedDiagnosticSession,
    SessionType_safetySystemDiagnosticSession
};


uint32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    struct __attribute__((packed)) {
        uint8_t sid;
        uint8_t diagnosticSessionType;
    } * receivedMessage = (void*)receiveBuffer;

    uint32_t transmitLength = 0;
    if (receiveBufferSize != 2u)
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer);
    }
    else
    {
        struct __attribute__((packed)) {
            uint8_t sid;
            uint8_t diagnosticSessionType;
            uint16_t timing;
            uint16_t enhancedTiming;
        } * transmitMessage = (void*)transmitBuffer;
        transmitMessage->sid = receivedMessage->sid | (uint8_t)uds_sid_PositiveResponseMask;
        transmitMessage->diagnosticSessionType = receivedMessage->diagnosticSessionType;
        transmitMessage->timing = __builtin_bswap16(50);
        transmitMessage->enhancedTiming = __builtin_bswap16(500);
        transmitLength = sizeof(*transmitMessage);
    }

    return transmitLength;
}
