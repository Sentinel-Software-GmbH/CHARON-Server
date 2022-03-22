/*
 * charon_InputOutputControlFunctionalUnit.h
 *
 *  Created on: 20.01.2020
 *      Author: Florian Kaup, Steven Inácio
 */

#ifndef CHARON_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_
#define CHARON_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_

#include "charon_types.h"
#include <stdbool.h>
#include <stdint.h>

#define STATIC_IO_CONTROL 0
#define DYNAMIC_IO_CONTROL 1

typedef struct IO_Ctrl_Object_t_public {
    bool (*returnControlToEcu)(void);
    bool (*resetToDefault)(void);
    bool (*freezeCurrentState)(void);
    bool (*shortTermAdjustment)(const uint8_t *buffer, uint32_t length);
} IO_Ctrl_Object_t;

#if STATIC_IO_CONTROL == 1 && DYNAMIC_IO_CONTROL == 1
    #error "Cannot activate both static and dynamic memory management."
#elif STATIC_IO_CONTROL == 1
    #define STATIC_IO_CONTROL_ARRAY_MAX_LENGTH 10
    typedef struct IO_Mask_Entry_t_public {
        uint16_t position;
        IO_Ctrl_Object_t ioObject;
    } IO_Mask_Entry_t;
    typedef struct IO_DID_Array_Entry_t_public {
        uint16_t DID;
        IO_Mask_Entry_t *io_mask_array;
        uint16_t io_mask_length;
        uint32_t data_size;
    } IO_DID_List_Entry_t;

    static IO_DID_List_Entry_t io_control_list[STATIC_IO_CONTROL_ARRAY_MAX_LENGTH];
    static uint16_t current_io_control_list_length = 0;
#elif DYNAMIC_IO_CONTROL == 1
    typedef struct IO_Mask_Entry_t_public {
        uint16_t position;
        IO_Ctrl_Object_t ioObject;
        struct IO_Mask_Entry_t_public *next;
    } IO_Mask_Entry_t;
    typedef struct IO_DID_List_Entry_t_public {
        uint16_t DID;
        IO_Mask_Entry_t *io_mask_list;
        uint16_t io_mask_length;
        uint32_t data_size;
        uint8_t *data;
        struct IO_DID_List_Entry_t_public *next;
    } IO_DID_List_Entry_t;

    bool charon_InputOutputControlFunctionalUnit_addIOEntry(IO_DID_List_Entry_t *entry);

    IO_DID_List_Entry_t* charon_InputOutputControlFunctionalUnit_removeIOEntry(uint16_t DID);

    bool charon_InputOutputControlFunctionalUnit_clearIOEntries();
#else
    #error "You must use either static or dynamic memory location for io control."
#endif

uds_responseCode_t charon_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier(const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* CHARON_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_ */
