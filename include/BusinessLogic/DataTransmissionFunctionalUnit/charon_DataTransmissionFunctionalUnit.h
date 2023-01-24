/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Andreas Hofmann
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
 * @defgroup BusinessLogic Business Logic Modules
 * @{
 * @file charon_DataTransmissionFunctionalUnit.h
 * Implementation of the DTF Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_DATATRANSMISSIONFUNCTIONALUNIT_H_
#define CHARON_DATATRANSMISSIONFUNCTIONALUNIT_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "charon_types.h"

/* Constants *****************************************************************/

/**
 * @brief 
 * A enumeration for the Send data Periodic fuction to set the speed or stop sending.
 */
typedef enum  transmissionMode_t_private
{
    sendSlowRate    = 0x01,     /**< Sends the Periodic Data at Slow Speed */
    sendMediumRate  = 0x02,     /**< Sends the Periodic Data at Medium Speed */
    sendFastRate    = 0x03,     /**< Sends the Periodic Data at Fast Speed */
    stopSending     = 0x04,     /**< Stops sending the Periodic Data */

    transmissionMode_amount
}transmissionMode_t;

/**
 * @brief 
 * A struct to keep all the Information for the Periodic sending function.
 */
typedef struct PeriodicData_t_private
{
    uint8_t             DID;                /**< Contains the Data IDentifier to send Periodic */
    transmissionMode_t  timing;             /**< contains the Information at witch speed the Periodic Data is send */
    bool                isEntryFree;        /**< contains if this entry is correctly in use or not */
    uint32_t            transmitCounter;    /**< Stores the counter to when send the next time */

}PeriodicData_t;

/**
 * @brief 
 * this Enumeration contains the Definition mode. 
 * And by witch method it is defined.
 */
typedef enum definitionMode_t_private
{
    defineByIdentifier          = 0x01,     /**< Define a Data Identifier by Identifier */
    defineByMemoryAddress       = 0x02,     /**< Define a Data Identifier by the Memory address */
    clearDynamicalDefinedDID    = 0x03,     /**< Clears the dynamicly defined Data Identifier */

    definitionMode_amount
}definitionMode_t;

/** 
 * @brief 
 * struct for unit test to get static set variables
*/
typedef struct periodicRateGetter_t_private
{
    uint32_t        slow_rate;      /**< used to captured the transmissionMode sendSlowRate, is used unitTest*/
    uint32_t        medium_rate;    /**< used to captured the transmissionMode sendMediumRate, is used unitTest*/
    uint32_t        fast_rate;      /**< used to captured the transmissionMode sendFastRate, is used unitTest*/
}periodicRateGetter_t;


/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/**
 * @brief 
 * initialises the Data Transmission Functional unit
 * clears the schedular and the array to be correct initialized.
 */
void charon_DataTransmissionFunctionalUnit_Init (void);

/** @brief The service allows the client to request data record values from the server identified by 
 *          one or more dataIdentifiers. 
 *          UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x22
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service allows the client to request memory data from the server via provided starting address and size of memory to be read.  
 *          UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x23
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service allows the client to request scaling data record information from the server identified by a dataIdentifier.  
 *          UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x24
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service allows the client to request the periodic transmission of data record 
 *          values from the server identified by one or more periodicDataIdentifiers.
 *          UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x2A
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service allows the client to dynamically define in a server a data identifier 
 *          that can be read via the ReadDataByIdentifier service at a later time. 
 *          UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x2C
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service allows the client to write information into the server at an internal location 
 *          specified by the provided data identifier.
 *          UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x2E
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service allows the client to write information into the server at one or more 
 *          contiguous memory locations. 
 *          UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x3D
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief 
 * Checks regularly if some periodic data a to send and sends them in the right intervals.
 */
void charon_DataTransmissionFunctionalUnit_SendPeriodic(void);

#ifdef TEST
/**
 * @brief Helper function for unit tests. Static variables can be read. For one or multiple request.
 * 
 * @return periodicRateGetter_t     struct filled with requested data.
 */
periodicRateGetter_t charon_DataTransmissionFunctionalUnit_getDefinedPeriodic (void);
#endif


#endif /* CHARON_DATATRANSMISSIONFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
