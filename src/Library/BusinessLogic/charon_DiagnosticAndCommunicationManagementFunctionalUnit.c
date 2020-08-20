/*
 * charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
 *
 *  Created on: 30.01.2020
 *      Author: Florian Kaup
 */

#include "charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "HSDI/charon_interface_debug.h"
#include "Common/charon_negativeResponse.h"

/**
 * Default timings for each session.
 * Sorted for their session id. Values are in milliseconds, for both parameters.
 */
// todo: these timing values are example values from iso 14229-2 chapter 7.2 table 4
// change these as necessary
static const struct {uint32_t p2, p2star;} defaultTimings[charon_sscType_amount] =
{
        {50,5000},
        {50,5000},
        {50,5000},
        {50,5000}
};

void charon_DiagnosticAndCommunicationManagementFunctionalUnit_reset (void)
{

}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uds_responseCode_t result = uds_responseCode_PositiveResponse;

    if (receiveBufferSize != 2u)
    {
        CHARON_ERROR("Unexpected message length.");
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else
    {
        uint8_t session = receiveBuffer[1] & 0x7Fu;
        uint8_t responseSuppress = receiveBuffer[1] & 0x80u;
        if ( (session >= (uint8_t)charon_sscType_amount) || (session == (uint8_t)charon_sscType_invalid) )
        {
            CHARON_WARNING("Session 0x%x unknown or invalid!", session);
            result = uds_responseCode_SubfunctionNotSupported;
        }
        else
        {
            CHARON_INFO("Changing Session to 0x%x.", session);

            if (responseSuppress == 0u)
            {
                struct {uint8_t sid, session; uint16_t p2, p2star;} __attribute__((packed)) transmitBuffer =
                {
                        (uint8_t)uds_sid_DiagnosticSessionControl | (uint8_t)uds_sid_PositiveResponseMask,
                        session,
                        (uint16_t)defaultTimings[session].p2,
                        (uint16_t)(defaultTimings[session].p2star/10u)
                };
                charon_sscTxMessage((uint8_t*)&transmitBuffer, sizeof(transmitBuffer));
            }
            charon_sscSetSession((charon_sessionTypes_t)session, defaultTimings[session].p2, defaultTimings[session].p2star);
        }
    }
    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, uds_sid_DiagnosticSessionControl);
    }
    return result;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uds_responseCode_t result = uds_responseCode_PositiveResponse;

    if (receiveBufferSize != 2u)
    {
        CHARON_ERROR("Unexpected message length.");
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if ( (receiveBuffer[1] & 0x7Fu) != 0u)
    {
        CHARON_ERROR("Subfunction is not 0x00 or 0x80.");
        result = uds_responseCode_SubfunctionNotSupported;
    }
    else
    {
        charon_sscTesterPresentHeartbeat();
        if ( (receiveBuffer[1] & 0x80u) == 0u)
        {
            uint8_t transmitBuffer[2] = {(uint8_t)uds_sid_TesterPresent | (uint8_t)uds_sid_PositiveResponseMask, 0u};
            charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
        }
    }

    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, uds_sid_TesterPresent);
    }
    return result;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    return uds_responseCode_ServiceNotSupported;
}

