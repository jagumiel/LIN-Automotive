/***************************************************************************//**
* \file LINS_SCB_SPI_UART_PVT.h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_LINS_SCB_H)
#define CY_SCB_SPI_UART_PVT_LINS_SCB_H

#include "LINS_SCB_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (LINS_SCB_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  LINS_SCB_rxBufferHead;
    extern volatile uint32  LINS_SCB_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   LINS_SCB_rxBufferOverflow;
    /** @} globals */
#endif /* (LINS_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

#if (LINS_SCB_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  LINS_SCB_txBufferHead;
    extern volatile uint32  LINS_SCB_txBufferTail;
#endif /* (LINS_SCB_INTERNAL_TX_SW_BUFFER_CONST) */

#if (LINS_SCB_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 LINS_SCB_rxBufferInternal[LINS_SCB_INTERNAL_RX_BUFFER_SIZE];
#endif /* (LINS_SCB_INTERNAL_RX_SW_BUFFER) */

#if (LINS_SCB_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 LINS_SCB_txBufferInternal[LINS_SCB_TX_BUFFER_SIZE];
#endif /* (LINS_SCB_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void LINS_SCB_SpiPostEnable(void);
void LINS_SCB_SpiStop(void);

#if (LINS_SCB_SCB_MODE_SPI_CONST_CFG)
    void LINS_SCB_SpiInit(void);
#endif /* (LINS_SCB_SCB_MODE_SPI_CONST_CFG) */

#if (LINS_SCB_SPI_WAKE_ENABLE_CONST)
    void LINS_SCB_SpiSaveConfig(void);
    void LINS_SCB_SpiRestoreConfig(void);
#endif /* (LINS_SCB_SPI_WAKE_ENABLE_CONST) */

void LINS_SCB_UartPostEnable(void);
void LINS_SCB_UartStop(void);

#if (LINS_SCB_SCB_MODE_UART_CONST_CFG)
    void LINS_SCB_UartInit(void);
#endif /* (LINS_SCB_SCB_MODE_UART_CONST_CFG) */

#if (LINS_SCB_UART_WAKE_ENABLE_CONST)
    void LINS_SCB_UartSaveConfig(void);
    void LINS_SCB_UartRestoreConfig(void);
#endif /* (LINS_SCB_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in LINS_SCB_SetPins() */
#define LINS_SCB_UART_RX_PIN_ENABLE    (LINS_SCB_UART_RX)
#define LINS_SCB_UART_TX_PIN_ENABLE    (LINS_SCB_UART_TX)

/* UART RTS and CTS position to be used in  LINS_SCB_SetPins() */
#define LINS_SCB_UART_RTS_PIN_ENABLE    (0x10u)
#define LINS_SCB_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define LINS_SCB_SpiUartEnableIntRx(intSourceMask)  LINS_SCB_SetRxInterruptMode(intSourceMask)
#define LINS_SCB_SpiUartEnableIntTx(intSourceMask)  LINS_SCB_SetTxInterruptMode(intSourceMask)
uint32  LINS_SCB_SpiUartDisableIntRx(void);
uint32  LINS_SCB_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_LINS_SCB_H) */


/* [] END OF FILE */
