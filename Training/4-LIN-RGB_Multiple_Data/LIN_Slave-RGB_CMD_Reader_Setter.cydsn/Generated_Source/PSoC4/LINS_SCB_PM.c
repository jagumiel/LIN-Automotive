/***************************************************************************//**
* \file LINS_SCB_PM.c
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

#include "LINS_SCB.h"
#include "LINS_SCB_PVT.h"

#if(LINS_SCB_SCB_MODE_I2C_INC)
    #include "LINS_SCB_I2C_PVT.h"
#endif /* (LINS_SCB_SCB_MODE_I2C_INC) */

#if(LINS_SCB_SCB_MODE_EZI2C_INC)
    #include "LINS_SCB_EZI2C_PVT.h"
#endif /* (LINS_SCB_SCB_MODE_EZI2C_INC) */

#if(LINS_SCB_SCB_MODE_SPI_INC || LINS_SCB_SCB_MODE_UART_INC)
    #include "LINS_SCB_SPI_UART_PVT.h"
#endif /* (LINS_SCB_SCB_MODE_SPI_INC || LINS_SCB_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
   (LINS_SCB_SCB_MODE_I2C_CONST_CFG   && (!LINS_SCB_I2C_WAKE_ENABLE_CONST))   || \
   (LINS_SCB_SCB_MODE_EZI2C_CONST_CFG && (!LINS_SCB_EZI2C_WAKE_ENABLE_CONST)) || \
   (LINS_SCB_SCB_MODE_SPI_CONST_CFG   && (!LINS_SCB_SPI_WAKE_ENABLE_CONST))   || \
   (LINS_SCB_SCB_MODE_UART_CONST_CFG  && (!LINS_SCB_UART_WAKE_ENABLE_CONST)))

    LINS_SCB_BACKUP_STRUCT LINS_SCB_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: LINS_SCB_Sleep
****************************************************************************//**
*
*  Prepares the LINS_SCB component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the LINS_SCB_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void LINS_SCB_Sleep(void)
{
#if(LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    if(LINS_SCB_SCB_WAKE_ENABLE_CHECK)
    {
        if(LINS_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            LINS_SCB_I2CSaveConfig();
        }
        else if(LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            LINS_SCB_EzI2CSaveConfig();
        }
    #if(!LINS_SCB_CY_SCBIP_V1)
        else if(LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
        {
            LINS_SCB_SpiSaveConfig();
        }
        else if(LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
        {
            LINS_SCB_UartSaveConfig();
        }
    #endif /* (!LINS_SCB_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        LINS_SCB_backup.enableState = (uint8) LINS_SCB_GET_CTRL_ENABLED;

        if(0u != LINS_SCB_backup.enableState)
        {
            LINS_SCB_Stop();
        }
    }

#else

    #if (LINS_SCB_SCB_MODE_I2C_CONST_CFG && LINS_SCB_I2C_WAKE_ENABLE_CONST)
        LINS_SCB_I2CSaveConfig();

    #elif (LINS_SCB_SCB_MODE_EZI2C_CONST_CFG && LINS_SCB_EZI2C_WAKE_ENABLE_CONST)
        LINS_SCB_EzI2CSaveConfig();

    #elif (LINS_SCB_SCB_MODE_SPI_CONST_CFG && LINS_SCB_SPI_WAKE_ENABLE_CONST)
        LINS_SCB_SpiSaveConfig();

    #elif (LINS_SCB_SCB_MODE_UART_CONST_CFG && LINS_SCB_UART_WAKE_ENABLE_CONST)
        LINS_SCB_UartSaveConfig();

    #else

        LINS_SCB_backup.enableState = (uint8) LINS_SCB_GET_CTRL_ENABLED;

        if(0u != LINS_SCB_backup.enableState)
        {
            LINS_SCB_Stop();
        }

    #endif /* defined (LINS_SCB_SCB_MODE_I2C_CONST_CFG) && (LINS_SCB_I2C_WAKE_ENABLE_CONST) */

#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LINS_SCB_Wakeup
****************************************************************************//**
*
*  Prepares the LINS_SCB component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the LINS_SCB_Wakeup() function without first calling the 
*   LINS_SCB_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void LINS_SCB_Wakeup(void)
{
#if(LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    if(LINS_SCB_SCB_WAKE_ENABLE_CHECK)
    {
        if(LINS_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            LINS_SCB_I2CRestoreConfig();
        }
        else if(LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            LINS_SCB_EzI2CRestoreConfig();
        }
    #if(!LINS_SCB_CY_SCBIP_V1)
        else if(LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
        {
            LINS_SCB_SpiRestoreConfig();
        }
        else if(LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
        {
            LINS_SCB_UartRestoreConfig();
        }
    #endif /* (!LINS_SCB_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != LINS_SCB_backup.enableState)
        {
            LINS_SCB_Enable();
        }
    }

#else

    #if (LINS_SCB_SCB_MODE_I2C_CONST_CFG  && LINS_SCB_I2C_WAKE_ENABLE_CONST)
        LINS_SCB_I2CRestoreConfig();

    #elif (LINS_SCB_SCB_MODE_EZI2C_CONST_CFG && LINS_SCB_EZI2C_WAKE_ENABLE_CONST)
        LINS_SCB_EzI2CRestoreConfig();

    #elif (LINS_SCB_SCB_MODE_SPI_CONST_CFG && LINS_SCB_SPI_WAKE_ENABLE_CONST)
        LINS_SCB_SpiRestoreConfig();

    #elif (LINS_SCB_SCB_MODE_UART_CONST_CFG && LINS_SCB_UART_WAKE_ENABLE_CONST)
        LINS_SCB_UartRestoreConfig();

    #else

        if(0u != LINS_SCB_backup.enableState)
        {
            LINS_SCB_Enable();
        }

    #endif /* (LINS_SCB_I2C_WAKE_ENABLE_CONST) */

#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
