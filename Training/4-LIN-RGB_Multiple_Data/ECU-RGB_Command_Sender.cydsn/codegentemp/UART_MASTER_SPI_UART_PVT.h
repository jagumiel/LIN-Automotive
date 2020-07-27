/***************************************************************************//**
* \file UART_MASTER_SPI_UART_PVT.h
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

#if !defined(CY_SCB_SPI_UART_PVT_UART_MASTER_H)
#define CY_SCB_SPI_UART_PVT_UART_MASTER_H

#include "UART_MASTER_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (UART_MASTER_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  UART_MASTER_rxBufferHead;
    extern volatile uint32  UART_MASTER_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   UART_MASTER_rxBufferOverflow;
    /** @} globals */
#endif /* (UART_MASTER_INTERNAL_RX_SW_BUFFER_CONST) */

#if (UART_MASTER_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  UART_MASTER_txBufferHead;
    extern volatile uint32  UART_MASTER_txBufferTail;
#endif /* (UART_MASTER_INTERNAL_TX_SW_BUFFER_CONST) */

#if (UART_MASTER_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 UART_MASTER_rxBufferInternal[UART_MASTER_INTERNAL_RX_BUFFER_SIZE];
#endif /* (UART_MASTER_INTERNAL_RX_SW_BUFFER) */

#if (UART_MASTER_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 UART_MASTER_txBufferInternal[UART_MASTER_TX_BUFFER_SIZE];
#endif /* (UART_MASTER_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void UART_MASTER_SpiPostEnable(void);
void UART_MASTER_SpiStop(void);

#if (UART_MASTER_SCB_MODE_SPI_CONST_CFG)
    void UART_MASTER_SpiInit(void);
#endif /* (UART_MASTER_SCB_MODE_SPI_CONST_CFG) */

#if (UART_MASTER_SPI_WAKE_ENABLE_CONST)
    void UART_MASTER_SpiSaveConfig(void);
    void UART_MASTER_SpiRestoreConfig(void);
#endif /* (UART_MASTER_SPI_WAKE_ENABLE_CONST) */

void UART_MASTER_UartPostEnable(void);
void UART_MASTER_UartStop(void);

#if (UART_MASTER_SCB_MODE_UART_CONST_CFG)
    void UART_MASTER_UartInit(void);
#endif /* (UART_MASTER_SCB_MODE_UART_CONST_CFG) */

#if (UART_MASTER_UART_WAKE_ENABLE_CONST)
    void UART_MASTER_UartSaveConfig(void);
    void UART_MASTER_UartRestoreConfig(void);
#endif /* (UART_MASTER_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in UART_MASTER_SetPins() */
#define UART_MASTER_UART_RX_PIN_ENABLE    (UART_MASTER_UART_RX)
#define UART_MASTER_UART_TX_PIN_ENABLE    (UART_MASTER_UART_TX)

/* UART RTS and CTS position to be used in  UART_MASTER_SetPins() */
#define UART_MASTER_UART_RTS_PIN_ENABLE    (0x10u)
#define UART_MASTER_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define UART_MASTER_SpiUartEnableIntRx(intSourceMask)  UART_MASTER_SetRxInterruptMode(intSourceMask)
#define UART_MASTER_SpiUartEnableIntTx(intSourceMask)  UART_MASTER_SetTxInterruptMode(intSourceMask)
uint32  UART_MASTER_SpiUartDisableIntRx(void);
uint32  UART_MASTER_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_UART_MASTER_H) */


/* [] END OF FILE */
