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
        if (session >= (uint8_t)charon_sscType_amount)
        {
            CHARON_WARNING("Session 0x%x unknown!", session);
            result = uds_responseCode_SubfunctionNotSupported;
        }
        else
        {
            CHARON_INFO("Changing Session to 0x%x.", session);
            // todo: these timing values are example values from iso 14229-1 chapter 15.4.2 table 445
            // change these as necessary
            if (responseSuppress == 0)
            {
                uint8_t transmitBuffer[6] = {(uint8_t)uds_sid_DiagnosticSessionControl | (uint8_t)uds_sid_PositiveResponseMask, session, 0, 0x96, 0x17, 0x70};
                charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
            }
            charon_sscSetSession((charon_sessionTypes_t)session, 0x96u, 0x1770uL*10u);
        }
    }
    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, uds_sid_RequestTransferExit);
    }
    return result;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return uds_responseCode_ServiceNotSupported;
}

