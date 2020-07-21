/***************************************************************************//**
* \file LINS_SCB_SPI_UART_INT.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LINS_SCB_PVT.h"
#include "LINS_SCB_SPI_UART_PVT.h"


#if (LINS_SCB_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: LINS_SCB_SPI_UART_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
*******************************************************************************/
CY_ISR(LINS_SCB_SPI_UART_ISR)
{
#if (LINS_SCB_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (LINS_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

#if (LINS_SCB_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (LINS_SCB_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef LINS_SCB_SPI_UART_ISR_ENTRY_CALLBACK
    LINS_SCB_SPI_UART_ISR_EntryCallback();
#endif /* LINS_SCB_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != LINS_SCB_customIntrHandler)
    {
        LINS_SCB_customIntrHandler();
    }

    #if(LINS_SCB_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        LINS_SCB_ClearSpiExtClkInterruptSource(LINS_SCB_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (LINS_SCB_CHECK_RX_SW_BUFFER)
    {
        if (LINS_SCB_CHECK_INTR_RX_MASKED(LINS_SCB_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (LINS_SCB_rxBufferHead + 1u);

                /* Adjust local head index */
                if (LINS_SCB_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == LINS_SCB_rxBufferTail)
                {
                    #if (LINS_SCB_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        LINS_SCB_INTR_RX_MASK_REG &= ~LINS_SCB_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) LINS_SCB_RX_FIFO_RD_REG;
                        LINS_SCB_rxBufferOverflow = (uint8) LINS_SCB_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    LINS_SCB_PutWordInRxBuffer(locHead, LINS_SCB_RX_FIFO_RD_REG);

                    /* Move head index */
                    LINS_SCB_rxBufferHead = locHead;
                }
            }
            while(0u != LINS_SCB_GET_RX_FIFO_ENTRIES);

            LINS_SCB_ClearRxInterruptSource(LINS_SCB_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (LINS_SCB_CHECK_TX_SW_BUFFER)
    {
        if (LINS_SCB_CHECK_INTR_TX_MASKED(LINS_SCB_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (LINS_SCB_txBufferHead != LINS_SCB_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (LINS_SCB_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (LINS_SCB_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    LINS_SCB_TX_FIFO_WR_REG = LINS_SCB_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    LINS_SCB_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    LINS_SCB_DISABLE_INTR_TX(LINS_SCB_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (LINS_SCB_SPI_UART_FIFO_SIZE != LINS_SCB_GET_TX_FIFO_ENTRIES);

            LINS_SCB_ClearTxInterruptSource(LINS_SCB_INTR_TX_NOT_FULL);
        }
    }
    #endif

#ifdef LINS_SCB_SPI_UART_ISR_EXIT_CALLBACK
    LINS_SCB_SPI_UART_ISR_ExitCallback();
#endif /* LINS_SCB_SPI_UART_ISR_EXIT_CALLBACK */

}

#endif /* (LINS_SCB_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
