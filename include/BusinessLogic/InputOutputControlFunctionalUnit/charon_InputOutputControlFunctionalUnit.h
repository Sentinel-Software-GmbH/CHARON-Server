/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Steven Inacio
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
 * @file charon_InputOutputControlFunctionalUnit.h
 * Module handles Service Group for Input and Output control functions
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/


#ifndef CHARON_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_
#define CHARON_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_

/* Includes ******************************************************************/

#include "charon_types.h"
#include <stdbool.h>
#include <stdint.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/
/** @brief Macro for easy differentiation of memory management using static memory assignment. */
#define STATIC_IO_CONTROL 0
/** @brief Macro for easy differentiation of memory management using dynamic memory assignment. */
#define DYNAMIC_IO_CONTROL 1

/* Types *********************************************************************/

/** @brief Contend of all available Input Output Control Parameter. */
typedef struct IO_Ctrl_Object_t_public 
{
    bool (*returnControlToEcu)(void);                                       /**< Return the Control of an IO DataIdentifier back to the ECU. */ 
    bool (*resetToDefault)(void);                                           /**< Return the IO DataIdentifier back to it's default Values. */
    bool (*freezeCurrentState)(void);                                       /**< Freeze the current state of an io data identifier. */
    bool (*shortTermAdjustment)(const uint8_t *buffer, uint32_t length);    /**< Adjust the io data identifier data. */
} IO_Ctrl_Object_t;

#if STATIC_IO_CONTROL == 1 && DYNAMIC_IO_CONTROL == 1
    #error "Cannot activate both static and dynamic memory management."
#elif STATIC_IO_CONTROL == 1
    #define STATIC_IO_CONTROL_ARRAY_MAX_LENGTH 10
    typedef struct IO_Mask_Entry_t_public 
    {
        uint16_t position;
        IO_Ctrl_Object_t ioObject;
    } IO_Mask_Entry_t;
    typedef struct IO_DID_Array_Entry_t_public 
    {
        uint16_t DID;
        IO_Mask_Entry_t *io_mask_array;
        uint16_t io_mask_length;
        uint32_t data_size;
    } IO_DID_List_Entry_t;

    static IO_DID_List_Entry_t io_control_list[STATIC_IO_CONTROL_ARRAY_MAX_LENGTH];
    static uint16_t current_io_control_list_length = 0;
#elif DYNAMIC_IO_CONTROL == 1
    /** @brief Construct Input Output Mask depending on Control Objects.*/
    typedef struct IO_Mask_Entry_t_public 
    {
        uint16_t position;                      /**< Position of entry in Mask. */
        IO_Ctrl_Object_t ioObject;              /**< @ref IO_Ctrl_Object_t. */
        struct IO_Mask_Entry_t_public *next;    /**< Pointer to next entry in list.*/    
    } IO_Mask_Entry_t;
    
    /** @brief Struct can be used to create a list of Input Output DIDs. */
    /** @todo limit the user from adding too many entries a counter could be introduced here.
        When the counter becomes greater than a threshold the function could return false then.
    */
    typedef struct IO_DID_List_Entry_t_public 
    {
        uint16_t DID;                               /**< DataIdentifier */
        IO_Mask_Entry_t *io_mask_list;              /**< Member of list*/
        uint16_t io_mask_length;                    /**< Length of IO Mask.*/
        uint32_t data_size;                         /**< Size of data buffer in bytes.*/    
        uint8_t *data;                              /**< Data buffer.*/        
        struct IO_DID_List_Entry_t_public *next;    /**< Pointer to next entry in list.*/
    } IO_DID_List_Entry_t;

#else
    #error "You must use either static or dynamic memory location for io control."
#endif

/* Interfaces ****************************************************************/

/** @brief Function is used to add new entries to DID list.
 * 
 * @param entry Entry that will be added to the list of DIDs. @ref IO_DID_List_Entry_t. 
 * @return True if entry could be added to list and False if maximum amount of entries reach its cap.
 * @warning Function does not check if there is a entry cap.
 */
bool charon_InputOutputControlFunctionalUnit_addIOEntry(IO_DID_List_Entry_t *entry);

/** @brief Function is used to remove entries from DID list.
 * 
 * @param DID DID that will be removed from DID list.
 * @return IO_DID_List_Entry_t* 
 */
IO_DID_List_Entry_t* charon_InputOutputControlFunctionalUnit_removeIOEntry(uint16_t DID);

/** @brief Function will empty DID list 
 * 
 * @return true if success.
 */
bool charon_InputOutputControlFunctionalUnit_clearIOEntries();

/** @brief The service is used by the client to substitute a value for an input signal,
 *         internal server function and/or force control to a value for an output (actuator) of an electronic system.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x2F
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */

uds_responseCode_t charon_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier(const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* CHARON_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
