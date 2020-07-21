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

#if !defined(CY_SCB_PVT_LINS_SCB_H)
#define CY_SCB_PVT_LINS_SCB_H

#include "LINS_SCB.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define LINS_SCB_SetI2CExtClkInterruptMode(interruptMask) LINS_SCB_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define LINS_SCB_ClearI2CExtClkInterruptSource(interruptMask) LINS_SCB_CLEAR_INTR_I2C_EC(interruptMask)
#define LINS_SCB_GetI2CExtClkInterruptSource()                (LINS_SCB_INTR_I2C_EC_REG)
#define LINS_SCB_GetI2CExtClkInterruptMode()                  (LINS_SCB_INTR_I2C_EC_MASK_REG)
#define LINS_SCB_GetI2CExtClkInterruptSourceMasked()          (LINS_SCB_INTR_I2C_EC_MASKED_REG)

#if (!LINS_SCB_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define LINS_SCB_SetSpiExtClkInterruptMode(interruptMask) \
                                                                LINS_SCB_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define LINS_SCB_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                LINS_SCB_CLEAR_INTR_SPI_EC(interruptMask)
    #define LINS_SCB_GetExtSpiClkInterruptSource()                 (LINS_SCB_INTR_SPI_EC_REG)
    #define LINS_SCB_GetExtSpiClkInterruptMode()                   (LINS_SCB_INTR_SPI_EC_MASK_REG)
    #define LINS_SCB_GetExtSpiClkInterruptSourceMasked()           (LINS_SCB_INTR_SPI_EC_MASKED_REG)
#endif /* (!LINS_SCB_CY_SCBIP_V1) */

#if(LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void LINS_SCB_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (LINS_SCB_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_LINS_SCB_CUSTOM_INTR_HANDLER)
    extern cyisraddress LINS_SCB_customIntrHandler;
#endif /* !defined (CY_REMOVE_LINS_SCB_CUSTOM_INTR_HANDLER) */
#endif /* (LINS_SCB_SCB_IRQ_INTERNAL) */

extern LINS_SCB_BACKUP_STRUCT LINS_SCB_backup;

#if(LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 LINS_SCB_scbMode;
    extern uint8 LINS_SCB_scbEnableWake;
    extern uint8 LINS_SCB_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 LINS_SCB_mode;
    extern uint8 LINS_SCB_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * LINS_SCB_rxBuffer;
    extern uint8   LINS_SCB_rxDataBits;
    extern uint32  LINS_SCB_rxBufferSize;

    extern volatile uint8 * LINS_SCB_txBuffer;
    extern uint8   LINS_SCB_txDataBits;
    extern uint32  LINS_SCB_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 LINS_SCB_numberOfAddr;
    extern uint8 LINS_SCB_subAddrSize;
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (LINS_SCB_SCB_MODE_I2C_CONST_CFG || \
        LINS_SCB_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 LINS_SCB_IntrTxMask;
#endif /* (! (LINS_SCB_SCB_MODE_I2C_CONST_CFG || \
              LINS_SCB_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define LINS_SCB_SCB_MODE_I2C_RUNTM_CFG     (LINS_SCB_SCB_MODE_I2C      == LINS_SCB_scbMode)
    #define LINS_SCB_SCB_MODE_SPI_RUNTM_CFG     (LINS_SCB_SCB_MODE_SPI      == LINS_SCB_scbMode)
    #define LINS_SCB_SCB_MODE_UART_RUNTM_CFG    (LINS_SCB_SCB_MODE_UART     == LINS_SCB_scbMode)
    #define LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG   (LINS_SCB_SCB_MODE_EZI2C    == LINS_SCB_scbMode)
    #define LINS_SCB_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (LINS_SCB_SCB_MODE_UNCONFIG == LINS_SCB_scbMode)

    /* Defines wakeup enable */
    #define LINS_SCB_SCB_WAKE_ENABLE_CHECK       (0u != LINS_SCB_scbEnableWake)
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!LINS_SCB_CY_SCBIP_V1)
    #define LINS_SCB_SCB_PINS_NUMBER    (7u)
#else
    #define LINS_SCB_SCB_PINS_NUMBER    (2u)
#endif /* (!LINS_SCB_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_LINS_SCB_H) */


/* [] END OF FILE */
