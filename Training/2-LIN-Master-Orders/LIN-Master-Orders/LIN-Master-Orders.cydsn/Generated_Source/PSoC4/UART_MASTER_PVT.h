/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
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

#if !defined(CY_SCB_PVT_UART_MASTER_H)
#define CY_SCB_PVT_UART_MASTER_H

#include "UART_MASTER.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define UART_MASTER_SetI2CExtClkInterruptMode(interruptMask) UART_MASTER_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define UART_MASTER_ClearI2CExtClkInterruptSource(interruptMask) UART_MASTER_CLEAR_INTR_I2C_EC(interruptMask)
#define UART_MASTER_GetI2CExtClkInterruptSource()                (UART_MASTER_INTR_I2C_EC_REG)
#define UART_MASTER_GetI2CExtClkInterruptMode()                  (UART_MASTER_INTR_I2C_EC_MASK_REG)
#define UART_MASTER_GetI2CExtClkInterruptSourceMasked()          (UART_MASTER_INTR_I2C_EC_MASKED_REG)

#if (!UART_MASTER_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define UART_MASTER_SetSpiExtClkInterruptMode(interruptMask) \
                                                                UART_MASTER_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define UART_MASTER_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                UART_MASTER_CLEAR_INTR_SPI_EC(interruptMask)
    #define UART_MASTER_GetExtSpiClkInterruptSource()                 (UART_MASTER_INTR_SPI_EC_REG)
    #define UART_MASTER_GetExtSpiClkInterruptMode()                   (UART_MASTER_INTR_SPI_EC_MASK_REG)
    #define UART_MASTER_GetExtSpiClkInterruptSourceMasked()           (UART_MASTER_INTR_SPI_EC_MASKED_REG)
#endif /* (!UART_MASTER_CY_SCBIP_V1) */

#if(UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void UART_MASTER_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (UART_MASTER_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_UART_MASTER_CUSTOM_INTR_HANDLER)
    extern cyisraddress UART_MASTER_customIntrHandler;
#endif /* !defined (CY_REMOVE_UART_MASTER_CUSTOM_INTR_HANDLER) */
#endif /* (UART_MASTER_SCB_IRQ_INTERNAL) */

extern UART_MASTER_BACKUP_STRUCT UART_MASTER_backup;

#if(UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 UART_MASTER_scbMode;
    extern uint8 UART_MASTER_scbEnableWake;
    extern uint8 UART_MASTER_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 UART_MASTER_mode;
    extern uint8 UART_MASTER_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * UART_MASTER_rxBuffer;
    extern uint8   UART_MASTER_rxDataBits;
    extern uint32  UART_MASTER_rxBufferSize;

    extern volatile uint8 * UART_MASTER_txBuffer;
    extern uint8   UART_MASTER_txDataBits;
    extern uint32  UART_MASTER_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 UART_MASTER_numberOfAddr;
    extern uint8 UART_MASTER_subAddrSize;
#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (UART_MASTER_SCB_MODE_I2C_CONST_CFG || \
        UART_MASTER_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 UART_MASTER_IntrTxMask;
#endif /* (! (UART_MASTER_SCB_MODE_I2C_CONST_CFG || \
              UART_MASTER_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define UART_MASTER_SCB_MODE_I2C_RUNTM_CFG     (UART_MASTER_SCB_MODE_I2C      == UART_MASTER_scbMode)
    #define UART_MASTER_SCB_MODE_SPI_RUNTM_CFG     (UART_MASTER_SCB_MODE_SPI      == UART_MASTER_scbMode)
    #define UART_MASTER_SCB_MODE_UART_RUNTM_CFG    (UART_MASTER_SCB_MODE_UART     == UART_MASTER_scbMode)
    #define UART_MASTER_SCB_MODE_EZI2C_RUNTM_CFG   (UART_MASTER_SCB_MODE_EZI2C    == UART_MASTER_scbMode)
    #define UART_MASTER_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (UART_MASTER_SCB_MODE_UNCONFIG == UART_MASTER_scbMode)

    /* Defines wakeup enable */
    #define UART_MASTER_SCB_WAKE_ENABLE_CHECK       (0u != UART_MASTER_scbEnableWake)
#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!UART_MASTER_CY_SCBIP_V1)
    #define UART_MASTER_SCB_PINS_NUMBER    (7u)
#else
    #define UART_MASTER_SCB_PINS_NUMBER    (2u)
#endif /* (!UART_MASTER_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_UART_MASTER_H) */


/* [] END OF FILE */
