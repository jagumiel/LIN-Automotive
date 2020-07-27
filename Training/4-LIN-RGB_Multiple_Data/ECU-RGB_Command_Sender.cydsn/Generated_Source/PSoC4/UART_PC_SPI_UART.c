/***************************************************************************//**
* \file UART_PC_SPI_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART_PC_PVT.h"
#include "UART_PC_SPI_UART_PVT.h"

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(UART_PC_INTERNAL_RX_SW_BUFFER_CONST)
    /* Start index to put data into the software receive buffer.*/
    volatile uint32 UART_PC_rxBufferHead;
    /* Start index to get data from the software receive buffer.*/
    volatile uint32 UART_PC_rxBufferTail;
    /**
    * \addtogroup group_globals
    * \{
    */
    /** Sets when internal software receive buffer overflow
    *  was occurred.
    */
    volatile uint8  UART_PC_rxBufferOverflow;
    /** \} globals */
#endif /* (UART_PC_INTERNAL_RX_SW_BUFFER_CONST) */

#if(UART_PC_INTERNAL_TX_SW_BUFFER_CONST)
    /* Start index to put data into the software transmit buffer.*/
    volatile uint32 UART_PC_txBufferHead;
    /* Start index to get data from the software transmit buffer.*/
    volatile uint32 UART_PC_txBufferTail;
#endif /* (UART_PC_INTERNAL_TX_SW_BUFFER_CONST) */

#if(UART_PC_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 UART_PC_rxBufferInternal[UART_PC_INTERNAL_RX_BUFFER_SIZE];
#endif /* (UART_PC_INTERNAL_RX_SW_BUFFER) */

#if(UART_PC_INTERNAL_TX_SW_BUFFER)
    volatile uint8 UART_PC_txBufferInternal[UART_PC_TX_BUFFER_SIZE];
#endif /* (UART_PC_INTERNAL_TX_SW_BUFFER) */


#if(UART_PC_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: UART_PC_SpiUartReadRxData
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * \return
    *  Next data element from the receive buffer. 
    *  The amount of data bits to be received depends on RX data bits selection 
    *  (the data bit counting starts from LSB of return value).
    *
    * \globalvars
    *  UART_PC_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  UART_PC_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 UART_PC_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

    #if (UART_PC_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (UART_PC_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (UART_PC_CHECK_RX_SW_BUFFER)
        {
            if (UART_PC_rxBufferHead != UART_PC_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (UART_PC_rxBufferTail + 1u);

                if (UART_PC_INTERNAL_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = UART_PC_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                UART_PC_rxBufferTail = locTail;

                #if (UART_PC_CHECK_UART_RTS_CONTROL_FLOW)
                {
                    /* Check if RX Not Empty is disabled in the interrupt */
                    if (0u == (UART_PC_INTR_RX_MASK_REG & UART_PC_INTR_RX_NOT_EMPTY))
                    {
                        /* Enable RX Not Empty interrupt source to continue
                        * receiving data into software buffer.
                        */
                        UART_PC_INTR_RX_MASK_REG |= UART_PC_INTR_RX_NOT_EMPTY;
                    }
                }
                #endif

            }
        }
        #else
        {
            /* Read data from RX FIFO */
            rxData = UART_PC_RX_FIFO_RD_REG;
        }
        #endif

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: UART_PC_SpiUartGetRxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * \return
    *  Number of received data elements.
    *
    * \globalvars
    *  UART_PC_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  UART_PC_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 UART_PC_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
    #if (UART_PC_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (UART_PC_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (UART_PC_CHECK_RX_SW_BUFFER)
        {
            locHead = UART_PC_rxBufferHead;

            if(locHead >= UART_PC_rxBufferTail)
            {
                size = (locHead - UART_PC_rxBufferTail);
            }
            else
            {
                size = (locHead + (UART_PC_INTERNAL_RX_BUFFER_SIZE - UART_PC_rxBufferTail));
            }
        }
        #else
        {
            size = UART_PC_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: UART_PC_SpiUartClearRxBuffer
    ****************************************************************************//**
    *
    *  Clears the receive buffer and RX FIFO.
    *
    * \globalvars
    *  UART_PC_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  UART_PC_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    void UART_PC_SpiUartClearRxBuffer(void)
    {
        #if (UART_PC_CHECK_RX_SW_BUFFER)
        {
            /* Lock from component interruption */
            UART_PC_DisableInt();

            /* Flush RX software buffer */
            UART_PC_rxBufferHead = UART_PC_rxBufferTail;
            UART_PC_rxBufferOverflow = 0u;

            UART_PC_CLEAR_RX_FIFO;
            UART_PC_ClearRxInterruptSource(UART_PC_INTR_RX_ALL);

            #if (UART_PC_CHECK_UART_RTS_CONTROL_FLOW)
            {
                /* Enable RX Not Empty interrupt source to continue receiving
                * data into software buffer.
                */
                UART_PC_INTR_RX_MASK_REG |= UART_PC_INTR_RX_NOT_EMPTY;
            }
            #endif
            
            /* Release lock */
            UART_PC_EnableInt();
        }
        #else
        {
            UART_PC_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (UART_PC_RX_DIRECTION) */


#if(UART_PC_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: UART_PC_SpiUartWriteTxData
    ****************************************************************************//**
    *
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *   The amount of data bits to be transmitted depends on TX data bits selection 
    *   (the data bit counting starts from LSB of txDataByte).
    *
    * \globalvars
    *  UART_PC_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  UART_PC_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void UART_PC_SpiUartWriteTxData(uint32 txData)
    {
    #if (UART_PC_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (UART_PC_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (UART_PC_CHECK_TX_SW_BUFFER)
        {
            /* Put data directly into the TX FIFO */
            if ((UART_PC_txBufferHead == UART_PC_txBufferTail) &&
                (UART_PC_SPI_UART_FIFO_SIZE != UART_PC_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                UART_PC_TX_FIFO_WR_REG = txData;
            }
            /* Put data into TX software buffer */
            else
            {
                /* Head index to put data */
                locHead = (UART_PC_txBufferHead + 1u);

                /* Adjust TX software buffer index */
                if (UART_PC_TX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                /* Wait for space in TX software buffer */
                while (locHead == UART_PC_txBufferTail)
                {
                }

                /* TX software buffer has at least one room */

                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                UART_PC_ClearTxInterruptSource(UART_PC_INTR_TX_NOT_FULL);

                UART_PC_PutWordInTxBuffer(locHead, txData);

                UART_PC_txBufferHead = locHead;

                /* Check if TX Not Full is disabled in interrupt */
                if (0u == (UART_PC_INTR_TX_MASK_REG & UART_PC_INTR_TX_NOT_FULL))
                {
                    /* Enable TX Not Full interrupt source to transmit from software buffer */
                    UART_PC_INTR_TX_MASK_REG |= (uint32) UART_PC_INTR_TX_NOT_FULL;
                }
            }
        }
        #else
        {
            /* Wait until TX FIFO has space to put data element */
            while (UART_PC_SPI_UART_FIFO_SIZE == UART_PC_GET_TX_FIFO_ENTRIES)
            {
            }

            UART_PC_TX_FIFO_WR_REG = txData;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: UART_PC_SpiUartPutArray
    ****************************************************************************//**
    *
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * \param wrBuf: pointer to an array of data to be placed in transmit buffer. 
    *  The width of the data to be transmitted depends on TX data width selection 
    *  (the data bit counting starts from LSB for each array element).
    * \param count: number of data elements to be placed in the transmit buffer.
    *
    * \globalvars
    *  UART_PC_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  UART_PC_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void UART_PC_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for (i=0u; i < count; i++)
        {
            UART_PC_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: UART_PC_SpiUartGetTxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of elements currently in the transmit buffer.
    *   - TX software buffer is disabled: returns the number of used entries in
    *     TX FIFO.
    *   - TX software buffer is enabled: returns the number of elements currently
    *     used in the transmit buffer. This number does not include used entries in
    *     the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *     not full.
    *
    * \return
    *  Number of data elements ready to transmit.
    *
    * \globalvars
    *  UART_PC_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  UART_PC_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    uint32 UART_PC_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
    #if (UART_PC_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (UART_PC_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (UART_PC_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = UART_PC_txBufferTail;

            if (UART_PC_txBufferHead >= locTail)
            {
                size = (UART_PC_txBufferHead - locTail);
            }
            else
            {
                size = (UART_PC_txBufferHead + (UART_PC_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = UART_PC_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: UART_PC_SpiUartClearTxBuffer
    ****************************************************************************//**
    *
    *  Clears the transmit buffer and TX FIFO.
    *
    * \globalvars
    *  UART_PC_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  UART_PC_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void UART_PC_SpiUartClearTxBuffer(void)
    {
        #if (UART_PC_CHECK_TX_SW_BUFFER)
        {
            /* Lock from component interruption */
            UART_PC_DisableInt();

            /* Flush TX software buffer */
            UART_PC_txBufferHead = UART_PC_txBufferTail;

            UART_PC_INTR_TX_MASK_REG &= (uint32) ~UART_PC_INTR_TX_NOT_FULL;
            UART_PC_CLEAR_TX_FIFO;
            UART_PC_ClearTxInterruptSource(UART_PC_INTR_TX_ALL);

            /* Release lock */
            UART_PC_EnableInt();
        }
        #else
        {
            UART_PC_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (UART_PC_TX_DIRECTION) */


/*******************************************************************************
* Function Name: UART_PC_SpiUartDisableIntRx
****************************************************************************//**
*
*  Disables the RX interrupt sources.
*
*  \return
*   Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 UART_PC_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = UART_PC_GetRxInterruptMode();

    UART_PC_SetRxInterruptMode(UART_PC_NO_INTR_SOURCES);

    return (intSource);
}


/*******************************************************************************
* Function Name: UART_PC_SpiUartDisableIntTx
****************************************************************************//**
*
*  Disables TX interrupt sources.
*
*  \return
*   Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 UART_PC_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = UART_PC_GetTxInterruptMode();

    UART_PC_SetTxInterruptMode(UART_PC_NO_INTR_SOURCES);

    return (intSourceMask);
}


#if(UART_PC_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: UART_PC_PutWordInRxBuffer
    ****************************************************************************//**
    *
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param index:      index to store data byte/word in the RX buffer.
    *  \param rxDataByte: byte/word to store.
    *
    *******************************************************************************/
    void UART_PC_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if (UART_PC_ONE_BYTE_WIDTH == UART_PC_rxDataBits)
        {
            UART_PC_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            UART_PC_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            UART_PC_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: UART_PC_GetWordFromRxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \return
    *   Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 UART_PC_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if (UART_PC_ONE_BYTE_WIDTH == UART_PC_rxDataBits)
        {
            value = UART_PC_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) UART_PC_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)UART_PC_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }


    /*******************************************************************************
    * Function Name: UART_PC_PutWordInTxBuffer
    ****************************************************************************//**
    *
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx:        index to store data byte/word in the TX buffer.
    *  \param txDataByte: byte/word to store.
    *
    *******************************************************************************/
    void UART_PC_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if (UART_PC_ONE_BYTE_WIDTH == UART_PC_txDataBits)
        {
            UART_PC_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            UART_PC_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            UART_PC_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: UART_PC_GetWordFromTxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx: index to get data byte/word from the TX buffer.
    *
    *  \return
    *   Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 UART_PC_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if (UART_PC_ONE_BYTE_WIDTH == UART_PC_txDataBits)
        {
            value = (uint32) UART_PC_txBuffer[idx];
        }
        else
        {
            value  = (uint32) UART_PC_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) UART_PC_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }

#endif /* (UART_PC_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
