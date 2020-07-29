/*******************************************************************************
* File Name: LedGreen.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "LedGreen.h"

static LedGreen_BACKUP_STRUCT  LedGreen_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: LedGreen_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet LedGreen_SUT.c usage_LedGreen_Sleep_Wakeup
*******************************************************************************/
void LedGreen_Sleep(void)
{
    #if defined(LedGreen__PC)
        LedGreen_backup.pcState = LedGreen_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            LedGreen_backup.usbState = LedGreen_CR1_REG;
            LedGreen_USB_POWER_REG |= LedGreen_USBIO_ENTER_SLEEP;
            LedGreen_CR1_REG &= LedGreen_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LedGreen__SIO)
        LedGreen_backup.sioState = LedGreen_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        LedGreen_SIO_REG &= (uint32)(~LedGreen_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: LedGreen_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to LedGreen_Sleep() for an example usage.
*******************************************************************************/
void LedGreen_Wakeup(void)
{
    #if defined(LedGreen__PC)
        LedGreen_PC = LedGreen_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            LedGreen_USB_POWER_REG &= LedGreen_USBIO_EXIT_SLEEP_PH1;
            LedGreen_CR1_REG = LedGreen_backup.usbState;
            LedGreen_USB_POWER_REG &= LedGreen_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LedGreen__SIO)
        LedGreen_SIO_REG = LedGreen_backup.sioState;
    #endif
}


/* [] END OF FILE */
