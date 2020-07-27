/*******************************************************************************
* File Name: LIN_1_INT.c
* Version 4.0
*
* Description:
*  This file contains the code that operates during the interrupt service
*  routine.
*
********************************************************************************
* Copyright 2011-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#include "LIN_1.h"
#include "LIN_Dynamic.h"
#include "cyapicallbacks.h"

CY_ISR(LIN_1_UART_ISR)
{
    #ifdef LIN_1_UART_ISR_ENTRY_CALLBACK
        LIN_1_UART_ISR_EntryCallback();
    #endif /* LIN_1_UART_ISR_ENTRY_CALLBACK */

    l_ifc_rx_LIN_1();

    #ifdef LIN_1_UART_ISR_EXIT_CALLBACK
        LIN_1_UART_ISR_ExitCallback();
    #endif /* LIN_1_UART_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
