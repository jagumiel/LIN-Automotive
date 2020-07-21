/*******************************************************************************
* File Name: LINS_INT.c
* Version 5.0
*
* Description:
*  This file provides the source code to the Interrupt Service Routine for
*  the LIN Slave component.
*
********************************************************************************
* Copyright 2011-2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#include "LINS.h"
#include "LIN_Dynamic.h"


CY_ISR(LINS_UART_ISR)
{
    #ifdef LINS_UART_ISR_ENTRY_CALLBACK
        LINS_UART_ISR_EntryCallback();
    #endif /* LINS_UART_ISR_ENTRY_CALLBACK */

    l_ifc_rx_LINS();

    #ifdef LINS_UART_ISR_EXIT_CALLBACK
        LINS_UART_ISR_ExitCallback();
    #endif /* LINS_UART_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
