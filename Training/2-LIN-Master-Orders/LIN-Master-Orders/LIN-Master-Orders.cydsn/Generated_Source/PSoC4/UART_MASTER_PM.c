/***************************************************************************//**
* \file UART_MASTER_PM.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
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

#include "UART_MASTER.h"
#include "UART_MASTER_PVT.h"

#if(UART_MASTER_SCB_MODE_I2C_INC)
    #include "UART_MASTER_I2C_PVT.h"
#endif /* (UART_MASTER_SCB_MODE_I2C_INC) */

#if(UART_MASTER_SCB_MODE_EZI2C_INC)
    #include "UART_MASTER_EZI2C_PVT.h"
#endif /* (UART_MASTER_SCB_MODE_EZI2C_INC) */

#if(UART_MASTER_SCB_MODE_SPI_INC || UART_MASTER_SCB_MODE_UART_INC)
    #include "UART_MASTER_SPI_UART_PVT.h"
#endif /* (UART_MASTER_SCB_MODE_SPI_INC || UART_MASTER_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG || \
   (UART_MASTER_SCB_MODE_I2C_CONST_CFG   && (!UART_MASTER_I2C_WAKE_ENABLE_CONST))   || \
   (UART_MASTER_SCB_MODE_EZI2C_CONST_CFG && (!UART_MASTER_EZI2C_WAKE_ENABLE_CONST)) || \
   (UART_MASTER_SCB_MODE_SPI_CONST_CFG   && (!UART_MASTER_SPI_WAKE_ENABLE_CONST))   || \
   (UART_MASTER_SCB_MODE_UART_CONST_CFG  && (!UART_MASTER_UART_WAKE_ENABLE_CONST)))

    UART_MASTER_BACKUP_STRUCT UART_MASTER_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: UART_MASTER_Sleep
****************************************************************************//**
*
*  Prepares the UART_MASTER component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the UART_MASTER_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void UART_MASTER_Sleep(void)
{
#if(UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)

    if(UART_MASTER_SCB_WAKE_ENABLE_CHECK)
    {
        if(UART_MASTER_SCB_MODE_I2C_RUNTM_CFG)
        {
            UART_MASTER_I2CSaveConfig();
        }
        else if(UART_MASTER_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            UART_MASTER_EzI2CSaveConfig();
        }
    #if(!UART_MASTER_CY_SCBIP_V1)
        else if(UART_MASTER_SCB_MODE_SPI_RUNTM_CFG)
        {
            UART_MASTER_SpiSaveConfig();
        }
        else if(UART_MASTER_SCB_MODE_UART_RUNTM_CFG)
        {
            UART_MASTER_UartSaveConfig();
        }
    #endif /* (!UART_MASTER_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        UART_MASTER_backup.enableState = (uint8) UART_MASTER_GET_CTRL_ENABLED;

        if(0u != UART_MASTER_backup.enableState)
        {
            UART_MASTER_Stop();
        }
    }

#else

    #if (UART_MASTER_SCB_MODE_I2C_CONST_CFG && UART_MASTER_I2C_WAKE_ENABLE_CONST)
        UART_MASTER_I2CSaveConfig();

    #elif (UART_MASTER_SCB_MODE_EZI2C_CONST_CFG && UART_MASTER_EZI2C_WAKE_ENABLE_CONST)
        UART_MASTER_EzI2CSaveConfig();

    #elif (UART_MASTER_SCB_MODE_SPI_CONST_CFG && UART_MASTER_SPI_WAKE_ENABLE_CONST)
        UART_MASTER_SpiSaveConfig();

    #elif (UART_MASTER_SCB_MODE_UART_CONST_CFG && UART_MASTER_UART_WAKE_ENABLE_CONST)
        UART_MASTER_UartSaveConfig();

    #else

        UART_MASTER_backup.enableState = (uint8) UART_MASTER_GET_CTRL_ENABLED;

        if(0u != UART_MASTER_backup.enableState)
        {
            UART_MASTER_Stop();
        }

    #endif /* defined (UART_MASTER_SCB_MODE_I2C_CONST_CFG) && (UART_MASTER_I2C_WAKE_ENABLE_CONST) */

#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: UART_MASTER_Wakeup
****************************************************************************//**
*
*  Prepares the UART_MASTER component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the UART_MASTER_Wakeup() function without first calling the 
*   UART_MASTER_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void UART_MASTER_Wakeup(void)
{
#if(UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)

    if(UART_MASTER_SCB_WAKE_ENABLE_CHECK)
    {
        if(UART_MASTER_SCB_MODE_I2C_RUNTM_CFG)
        {
            UART_MASTER_I2CRestoreConfig();
        }
        else if(UART_MASTER_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            UART_MASTER_EzI2CRestoreConfig();
        }
    #if(!UART_MASTER_CY_SCBIP_V1)
        else if(UART_MASTER_SCB_MODE_SPI_RUNTM_CFG)
        {
            UART_MASTER_SpiRestoreConfig();
        }
        else if(UART_MASTER_SCB_MODE_UART_RUNTM_CFG)
        {
            UART_MASTER_UartRestoreConfig();
        }
    #endif /* (!UART_MASTER_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != UART_MASTER_backup.enableState)
        {
            UART_MASTER_Enable();
        }
    }

#else

    #if (UART_MASTER_SCB_MODE_I2C_CONST_CFG  && UART_MASTER_I2C_WAKE_ENABLE_CONST)
        UART_MASTER_I2CRestoreConfig();

    #elif (UART_MASTER_SCB_MODE_EZI2C_CONST_CFG && UART_MASTER_EZI2C_WAKE_ENABLE_CONST)
        UART_MASTER_EzI2CRestoreConfig();

    #elif (UART_MASTER_SCB_MODE_SPI_CONST_CFG && UART_MASTER_SPI_WAKE_ENABLE_CONST)
        UART_MASTER_SpiRestoreConfig();

    #elif (UART_MASTER_SCB_MODE_UART_CONST_CFG && UART_MASTER_UART_WAKE_ENABLE_CONST)
        UART_MASTER_UartRestoreConfig();

    #else

        if(0u != UART_MASTER_backup.enableState)
        {
            UART_MASTER_Enable();
        }

    #endif /* (UART_MASTER_I2C_WAKE_ENABLE_CONST) */

#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
