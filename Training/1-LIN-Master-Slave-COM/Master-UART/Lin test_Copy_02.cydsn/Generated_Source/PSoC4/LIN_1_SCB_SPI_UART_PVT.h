/***************************************************************************//**
* \file LIN_1_SCB_SPI_UART_PVT.h
* \version 3.20
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
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_LIN_1_SCB_H)
#define CY_SCB_SPI_UART_PVT_LIN_1_SCB_H

#include "LIN_1_SCB_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (LIN_1_SCB_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  LIN_1_SCB_rxBufferHead;
    extern volatile uint32  LIN_1_SCB_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   LIN_1_SCB_rxBufferOverflow;
    /** @} globals */
#endif /* (LIN_1_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

#if (LIN_1_SCB_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  LIN_1_SCB_txBufferHead;
    extern volatile uint32  LIN_1_SCB_txBufferTail;
#endif /* (LIN_1_SCB_INTERNAL_TX_SW_BUFFER_CONST) */

#if (LIN_1_SCB_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 LIN_1_SCB_rxBufferInternal[LIN_1_SCB_INTERNAL_RX_BUFFER_SIZE];
#endif /* (LIN_1_SCB_INTERNAL_RX_SW_BUFFER) */

#if (LIN_1_SCB_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 LIN_1_SCB_txBufferInternal[LIN_1_SCB_TX_BUFFER_SIZE];
#endif /* (LIN_1_SCB_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void LIN_1_SCB_SpiPostEnable(void);
void LIN_1_SCB_SpiStop(void);

#if (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG)
    void LIN_1_SCB_SpiInit(void);
#endif /* (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG) */

#if (LIN_1_SCB_SPI_WAKE_ENABLE_CONST)
    void LIN_1_SCB_SpiSaveConfig(void);
    void LIN_1_SCB_SpiRestoreConfig(void);
#endif /* (LIN_1_SCB_SPI_WAKE_ENABLE_CONST) */

void LIN_1_SCB_UartPostEnable(void);
void LIN_1_SCB_UartStop(void);

#if (LIN_1_SCB_SCB_MODE_UART_CONST_CFG)
    void LIN_1_SCB_UartInit(void);
#endif /* (LIN_1_SCB_SCB_MODE_UART_CONST_CFG) */

#if (LIN_1_SCB_UART_WAKE_ENABLE_CONST)
    void LIN_1_SCB_UartSaveConfig(void);
    void LIN_1_SCB_UartRestoreConfig(void);
#endif /* (LIN_1_SCB_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in LIN_1_SCB_SetPins() */
#define LIN_1_SCB_UART_RX_PIN_ENABLE    (LIN_1_SCB_UART_RX)
#define LIN_1_SCB_UART_TX_PIN_ENABLE    (LIN_1_SCB_UART_TX)

/* UART RTS and CTS position to be used in  LIN_1_SCB_SetPins() */
#define LIN_1_SCB_UART_RTS_PIN_ENABLE    (0x10u)
#define LIN_1_SCB_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define LIN_1_SCB_SpiUartEnableIntRx(intSourceMask)  LIN_1_SCB_SetRxInterruptMode(intSourceMask)
#define LIN_1_SCB_SpiUartEnableIntTx(intSourceMask)  LIN_1_SCB_SetTxInterruptMode(intSourceMask)
uint32  LIN_1_SCB_SpiUartDisableIntRx(void);
uint32  LIN_1_SCB_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_LIN_1_SCB_H) */


/* [] END OF FILE */
