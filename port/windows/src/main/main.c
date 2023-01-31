/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
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
 * @file main.c
 *  Charon Server Windows port implementation. 
 *  Can be used as example code.
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "charon_uds.h"
#include "ISocket.h"
#include "charon_interface_debug.h"

#if CHARON_CONFIG_LOAD_DUMMY_DATA
#include "showcaseData.h"
#endif

#if CHARON_CONFIG_DEBUGGING_SERVER_INTERN
#include "charon_DataLookupTable.h"
#include "charon_DataTransmissionFunctionalUnit.h"
#include "charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "charon_DTC_LookupTable.h"
#include "charon_InputOutputControlFunctionalUnit.h"
#include "charon_RoutineFunctionalUnit.h"
#include "charon_StoredDataTransmissionFunctionalUnit.h"
#include "charon_UploadDownloadFunctionalUnit.h"
#include "charon_ServiceLookupTable.h"
#include "charon_SessionAndServiceControl.h"
#endif


/* Imports *******************************************************************/

/** @brief Function to initialize pipe for communication between Client and Server when both entities launched on windows. */ 
extern void pipe_init (void);
/** @brief abstract windows hardware Interface for communication. */
extern ISocket_t pipe_socket;

/** @brief Function to initialize pipe for communication between Client and Server when communication via uart. */ 
extern void uart_init (void);
/** @brief abstract uart hardware Interface for communication. */
extern ISocket_t uart_socket;

/** @brief abstract windows terminal Interface for communication. does not need an init. */
extern ISocket_t terminal_socket;


/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

#if CHARON_CONFIG_DEBUGGING_SERVER_INTERN
/**
 * @brief Used for debugging server intern, preferably done with direct Client <-> Server communication.
 */
static void debug (void);
#endif


/* Interfaces  ***************************************************************/

/** @brief main function for charon windows port */
int main (void)
{
    pipe_init();

#if !DEBUG
    // default
    charon_init(pipe_socket);
#endif
#if DEBUG
        // default
    #if !CHARON_CONFIG_DEBUGGING_SERVER_INTERN
        charon_init(pipe_socket);
        #if CHARON_CONFIG_LOAD_DUMMY_DATA
        DTC_dataPackage();


        #endif
    #endif

        // intern test
    #if CHARON_CONFIG_DEBUGGING_SERVER_INTERN
        charon_init(terminal_socket);
        #if CHARON_CONFIG_LOAD_DUMMY_DATA
        DTC_dataPackage();
        #endif
        debug();
    #endif
#endif 

    CHARON_INFO("CharonUDS Initialized...");
    while (1)
    {
        charon_task();
    }
    return 0;
}


/* Private Function **********************************************************/

#if CHARON_CONFIG_DEBUGGING_SERVER_INTERN
static void debug (void)
{
    /* Add your function to debug/test here. */
}
#endif 


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/