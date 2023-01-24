/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2023 René Mutscheller
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
 * @addtogroup WindowsPort Windows porting
 * @{
 * @file showcaseData.c
 * Showcase data for tests and debugging.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "charon_StoredDataTransmissionFunctionalUnit.h"
#include "charon_interface_NvmDriver.h"
#include "charon_DTC_LookupTable.h"
#include "showcaseData.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void DTC_dataPackage (void)
{  
    charon_DTC_LookupTable_header_SET();

    DTC_t DTC_demo;
    DTC_SnapshotData_t snap_demo;
    DTC_StoredData_t stored_demo;
    DTC_ExtendedData_t ext_demo;
    uint8_t demo_number_1 = 0x00;
    uint8_t demo_number_2 = 0x44;
    uint8_t demo_number_3;


    snap_demo.DTCSnapshotDataRecordNumberOfIdentifiers = 0x01;
    snap_demo.DTCSnapshotDataPayload[0] = 0xEE;
    DTC_demo.DTCSnapshotLength[0] = 0x01;

    stored_demo.DTCStoredDataRecordNumberOfIdentifiers = 0x01;
    stored_demo.DTCStoredDataPayload[0] = 0xEE;
    DTC_demo.DTCStoredDataLength[0] = 0x01;

    ext_demo.DTCExtendedDataRecordNumberOfIdentifiers = 0x01;
    ext_demo.DTCExtendedDataPayload[0] = 0xEE;
    DTC_demo.DTCExtendedDataLength[0] = 0x01;

    for (uint32_t i = 0; i < 98; i++)
    {
        DTC_demo.DTCStatusMask = 0xFF;

        demo_number_3 = (i % 3);
        switch (demo_number_3)
        {
        case 0x01:
            DTC_demo.DTCStatusMask = 0x30;
            break;

        case 0x02:
            DTC_demo.DTCStatusMask = 0x2F;
            break;
        
        default:
            DTC_demo.DTCStatusMask = 0x63;
            break;
        }


        if (demo_number_1 >= 0xF0)
        {
            DTC_demo.DTCHighByte = demo_number_1;
        }
        else
        {
            DTC_demo.DTCHighByte = (demo_number_1 / 0x03);
        }
        DTC_demo.DTCMiddleByte = demo_number_2;
        DTC_demo.DTCLowByte = i;


        demo_number_3 = (i % 2);
        switch (demo_number_3)
        {
        case 0x01:
            DTC_demo.DTCSeverityMask = 0x1F;
            break;
     
        default:
            DTC_demo.DTCSeverityMask = 0x03;
            break;
        }


        DTC_demo.DTCSeverityMaskRecordHigh = DTC_demo.DTCSeverityMask;
        DTC_demo.DTCSeverityMaskRecordLow =  DTC_demo.DTCStatusMask;


        demo_number_3 = (i % 3);
        switch (demo_number_3)
        {
        case 0x01:
            DTC_demo.FunctionalGroupIdentifier = 0x33;
            break;

        case 0x02:
            DTC_demo.FunctionalGroupIdentifier = 0xD0;
            break;
        
        default:
            DTC_demo.FunctionalGroupIdentifier = 0xFE;
            break;
        }


        DTC_demo.statusOfDTC = DTC_demo.DTCStatusMask;
        DTC_demo.DTCSettingType = 0x01; // 0x01: updating = ON, 0x02: updating = OFF
        DTC_demo.MemorySelection = 0x00;


        charon_StoredDataTransmissionFunctionalUnit_writeDTCToNvm(DTC_demo,snap_demo,stored_demo,ext_demo);

        demo_number_1 += 0x07;
        demo_number_2 += 0x09;
    }


    snap_demo.DTCSnapshotDataRecordNumberOfIdentifiers = 0x08;
    for (uint32_t i = 0; i < 8; i+=2)
    {
        snap_demo.DTCSnapshotDataPayload[i] = (0x1B + i);
        snap_demo.DTCSnapshotDataPayload[i+1] = (i * 3) +1;
    }
    DTC_demo.DTCSnapshotLength[0] = 0x08;
    DTC_demo.DTCHighByte = 0xF8;
    DTC_demo.DTCMiddleByte = 0xCC;
    DTC_demo.DTCLowByte = 0x48;
    charon_StoredDataTransmissionFunctionalUnit_writeDTCToNvm(DTC_demo,snap_demo,stored_demo,ext_demo);
}


/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
