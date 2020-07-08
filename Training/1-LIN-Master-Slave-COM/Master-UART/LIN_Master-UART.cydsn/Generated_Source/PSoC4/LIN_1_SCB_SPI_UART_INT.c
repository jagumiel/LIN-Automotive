/***************************************************************************//**
* \file LIN_1_SCB_SPI_UART_INT.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LIN_1_SCB_PVT.h"
#include "LIN_1_SCB_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (LIN_1_SCB_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: LIN_1_SCB_SPI_UART_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
*******************************************************************************/
CY_ISR(LIN_1_SCB_SPI_UART_ISR)
{
#if (LIN_1_SCB_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (LIN_1_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

#if (LIN_1_SCB_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (LIN_1_SCB_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef LIN_1_SCB_SPI_UART_ISR_ENTRY_CALLBACK
    LIN_1_SCB_SPI_UART_ISR_EntryCallback();
#endif /* LIN_1_SCB_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != LIN_1_SCB_customIntrHandler)
    {
        LIN_1_SCB_customIntrHandler();
    }

    #if(LIN_1_SCB_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        LIN_1_SCB_ClearSpiExtClkInterruptSource(LIN_1_SCB_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (LIN_1_SCB_CHECK_RX_SW_BUFFER)
    {
        if (LIN_1_SCB_CHECK_INTR_RX_MASKED(LIN_1_SCB_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (LIN_1_SCB_rxBufferHead + 1u);

                /* Adjust local head index */
                if (LIN_1_SCB_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == LIN_1_SCB_rxBufferTail)
                {
                    #if (LIN_1_SCB_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        LIN_1_SCB_INTR_RX_MASK_REG &= ~LIN_1_SCB_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) LIN_1_SCB_RX_FIFO_RD_REG;
                        LIN_1_SCB_rxBufferOverflow = (uint8) LIN_1_SCB_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    LIN_1_SCB_PutWordInRxBuffer(locHead, LIN_1_SCB_RX_FIFO_RD_REG);

                    /* Move head index */
                    LIN_1_SCB_rxBufferHead = locHead;
                }
            }
            while(0u != LIN_1_SCB_GET_RX_FIFO_ENTRIES);

            LIN_1_SCB_ClearRxInterruptSource(LIN_1_SCB_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (LIN_1_SCB_CHECK_TX_SW_BUFFER)
    {
        if (LIN_1_SCB_CHECK_INTR_TX_MASKED(LIN_1_SCB_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (LIN_1_SCB_txBufferHead != LIN_1_SCB_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (LIN_1_SCB_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (LIN_1_SCB_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    LIN_1_SCB_TX_FIFO_WR_REG = LIN_1_SCB_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    LIN_1_SCB_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    LIN_1_SCB_DISABLE_INTR_TX(LIN_1_SCB_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (LIN_1_SCB_SPI_UART_FIFO_SIZE != LIN_1_SCB_GET_TX_FIFO_ENTRIES);

            LIN_1_SCB_ClearTxInterruptSource(LIN_1_SCB_INTR_TX_NOT_FULL);
        }
    }
    #endif

#ifdef LIN_1_SCB_SPI_UART_ISR_EXIT_CALLBACK
    LIN_1_SCB_SPI_UART_ISR_ExitCallback();
#endif /* LIN_1_SCB_SPI_UART_ISR_EXIT_CALLBACK */

}

#endif /* (LIN_1_SCB_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
