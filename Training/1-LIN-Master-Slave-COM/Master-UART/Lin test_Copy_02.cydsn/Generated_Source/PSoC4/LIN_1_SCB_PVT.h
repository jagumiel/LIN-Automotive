/***************************************************************************//**
* \file .h
* \version 3.20
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
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_LIN_1_SCB_H)
#define CY_SCB_PVT_LIN_1_SCB_H

#include "LIN_1_SCB.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define LIN_1_SCB_SetI2CExtClkInterruptMode(interruptMask) LIN_1_SCB_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define LIN_1_SCB_ClearI2CExtClkInterruptSource(interruptMask) LIN_1_SCB_CLEAR_INTR_I2C_EC(interruptMask)
#define LIN_1_SCB_GetI2CExtClkInterruptSource()                (LIN_1_SCB_INTR_I2C_EC_REG)
#define LIN_1_SCB_GetI2CExtClkInterruptMode()                  (LIN_1_SCB_INTR_I2C_EC_MASK_REG)
#define LIN_1_SCB_GetI2CExtClkInterruptSourceMasked()          (LIN_1_SCB_INTR_I2C_EC_MASKED_REG)

#if (!LIN_1_SCB_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define LIN_1_SCB_SetSpiExtClkInterruptMode(interruptMask) \
                                                                LIN_1_SCB_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define LIN_1_SCB_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                LIN_1_SCB_CLEAR_INTR_SPI_EC(interruptMask)
    #define LIN_1_SCB_GetExtSpiClkInterruptSource()                 (LIN_1_SCB_INTR_SPI_EC_REG)
    #define LIN_1_SCB_GetExtSpiClkInterruptMode()                   (LIN_1_SCB_INTR_SPI_EC_MASK_REG)
    #define LIN_1_SCB_GetExtSpiClkInterruptSourceMasked()           (LIN_1_SCB_INTR_SPI_EC_MASKED_REG)
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */

#if(LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void LIN_1_SCB_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (LIN_1_SCB_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_LIN_1_SCB_CUSTOM_INTR_HANDLER)
    extern cyisraddress LIN_1_SCB_customIntrHandler;
#endif /* !defined (CY_REMOVE_LIN_1_SCB_CUSTOM_INTR_HANDLER) */
#endif /* (LIN_1_SCB_SCB_IRQ_INTERNAL) */

extern LIN_1_SCB_BACKUP_STRUCT LIN_1_SCB_backup;

#if(LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 LIN_1_SCB_scbMode;
    extern uint8 LIN_1_SCB_scbEnableWake;
    extern uint8 LIN_1_SCB_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 LIN_1_SCB_mode;
    extern uint8 LIN_1_SCB_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * LIN_1_SCB_rxBuffer;
    extern uint8   LIN_1_SCB_rxDataBits;
    extern uint32  LIN_1_SCB_rxBufferSize;

    extern volatile uint8 * LIN_1_SCB_txBuffer;
    extern uint8   LIN_1_SCB_txDataBits;
    extern uint32  LIN_1_SCB_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 LIN_1_SCB_numberOfAddr;
    extern uint8 LIN_1_SCB_subAddrSize;
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG || \
        LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 LIN_1_SCB_IntrTxMask;
#endif /* (! (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG || \
              LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG     (LIN_1_SCB_SCB_MODE_I2C      == LIN_1_SCB_scbMode)
    #define LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG     (LIN_1_SCB_SCB_MODE_SPI      == LIN_1_SCB_scbMode)
    #define LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG    (LIN_1_SCB_SCB_MODE_UART     == LIN_1_SCB_scbMode)
    #define LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG   (LIN_1_SCB_SCB_MODE_EZI2C    == LIN_1_SCB_scbMode)
    #define LIN_1_SCB_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (LIN_1_SCB_SCB_MODE_UNCONFIG == LIN_1_SCB_scbMode)

    /* Defines wakeup enable */
    #define LIN_1_SCB_SCB_WAKE_ENABLE_CHECK       (0u != LIN_1_SCB_scbEnableWake)
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!LIN_1_SCB_CY_SCBIP_V1)
    #define LIN_1_SCB_SCB_PINS_NUMBER    (7u)
#else
    #define LIN_1_SCB_SCB_PINS_NUMBER    (2u)
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_LIN_1_SCB_H) */


/* [] END OF FILE */
