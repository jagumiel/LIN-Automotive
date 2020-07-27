/***************************************************************************//**
* \file LIN_1_SCB_PM.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
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

#include "LIN_1_SCB.h"
#include "LIN_1_SCB_PVT.h"

#if(LIN_1_SCB_SCB_MODE_I2C_INC)
    #include "LIN_1_SCB_I2C_PVT.h"
#endif /* (LIN_1_SCB_SCB_MODE_I2C_INC) */

#if(LIN_1_SCB_SCB_MODE_EZI2C_INC)
    #include "LIN_1_SCB_EZI2C_PVT.h"
#endif /* (LIN_1_SCB_SCB_MODE_EZI2C_INC) */

#if(LIN_1_SCB_SCB_MODE_SPI_INC || LIN_1_SCB_SCB_MODE_UART_INC)
    #include "LIN_1_SCB_SPI_UART_PVT.h"
#endif /* (LIN_1_SCB_SCB_MODE_SPI_INC || LIN_1_SCB_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
   (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG   && (!LIN_1_SCB_I2C_WAKE_ENABLE_CONST))   || \
   (LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG && (!LIN_1_SCB_EZI2C_WAKE_ENABLE_CONST)) || \
   (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG   && (!LIN_1_SCB_SPI_WAKE_ENABLE_CONST))   || \
   (LIN_1_SCB_SCB_MODE_UART_CONST_CFG  && (!LIN_1_SCB_UART_WAKE_ENABLE_CONST)))

    LIN_1_SCB_BACKUP_STRUCT LIN_1_SCB_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: LIN_1_SCB_Sleep
****************************************************************************//**
*
*  Prepares the LIN_1_SCB component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the LIN_1_SCB_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void LIN_1_SCB_Sleep(void)
{
#if(LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    if(LIN_1_SCB_SCB_WAKE_ENABLE_CHECK)
    {
        if(LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            LIN_1_SCB_I2CSaveConfig();
        }
        else if(LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            LIN_1_SCB_EzI2CSaveConfig();
        }
    #if(!LIN_1_SCB_CY_SCBIP_V1)
        else if(LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
        {
            LIN_1_SCB_SpiSaveConfig();
        }
        else if(LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
        {
            LIN_1_SCB_UartSaveConfig();
        }
    #endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        LIN_1_SCB_backup.enableState = (uint8) LIN_1_SCB_GET_CTRL_ENABLED;

        if(0u != LIN_1_SCB_backup.enableState)
        {
            LIN_1_SCB_Stop();
        }
    }

#else

    #if (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG && LIN_1_SCB_I2C_WAKE_ENABLE_CONST)
        LIN_1_SCB_I2CSaveConfig();

    #elif (LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG && LIN_1_SCB_EZI2C_WAKE_ENABLE_CONST)
        LIN_1_SCB_EzI2CSaveConfig();

    #elif (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG && LIN_1_SCB_SPI_WAKE_ENABLE_CONST)
        LIN_1_SCB_SpiSaveConfig();

    #elif (LIN_1_SCB_SCB_MODE_UART_CONST_CFG && LIN_1_SCB_UART_WAKE_ENABLE_CONST)
        LIN_1_SCB_UartSaveConfig();

    #else

        LIN_1_SCB_backup.enableState = (uint8) LIN_1_SCB_GET_CTRL_ENABLED;

        if(0u != LIN_1_SCB_backup.enableState)
        {
            LIN_1_SCB_Stop();
        }

    #endif /* defined (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG) && (LIN_1_SCB_I2C_WAKE_ENABLE_CONST) */

#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LIN_1_SCB_Wakeup
****************************************************************************//**
*
*  Prepares the LIN_1_SCB component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the LIN_1_SCB_Wakeup() function without first calling the 
*   LIN_1_SCB_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void LIN_1_SCB_Wakeup(void)
{
#if(LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    if(LIN_1_SCB_SCB_WAKE_ENABLE_CHECK)
    {
        if(LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            LIN_1_SCB_I2CRestoreConfig();
        }
        else if(LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            LIN_1_SCB_EzI2CRestoreConfig();
        }
    #if(!LIN_1_SCB_CY_SCBIP_V1)
        else if(LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
        {
            LIN_1_SCB_SpiRestoreConfig();
        }
        else if(LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
        {
            LIN_1_SCB_UartRestoreConfig();
        }
    #endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != LIN_1_SCB_backup.enableState)
        {
            LIN_1_SCB_Enable();
        }
    }

#else

    #if (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG  && LIN_1_SCB_I2C_WAKE_ENABLE_CONST)
        LIN_1_SCB_I2CRestoreConfig();

    #elif (LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG && LIN_1_SCB_EZI2C_WAKE_ENABLE_CONST)
        LIN_1_SCB_EzI2CRestoreConfig();

    #elif (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG && LIN_1_SCB_SPI_WAKE_ENABLE_CONST)
        LIN_1_SCB_SpiRestoreConfig();

    #elif (LIN_1_SCB_SCB_MODE_UART_CONST_CFG && LIN_1_SCB_UART_WAKE_ENABLE_CONST)
        LIN_1_SCB_UartRestoreConfig();

    #else

        if(0u != LIN_1_SCB_backup.enableState)
        {
            LIN_1_SCB_Enable();
        }

    #endif /* (LIN_1_SCB_I2C_WAKE_ENABLE_CONST) */

#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
