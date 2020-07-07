/*******************************************************************************
* File Name: SW3.c  
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
#include "SW3.h"

static SW3_BACKUP_STRUCT  SW3_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: SW3_Sleep
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
*  \snippet SW3_SUT.c usage_SW3_Sleep_Wakeup
*******************************************************************************/
void SW3_Sleep(void)
{
    #if defined(SW3__PC)
        SW3_backup.pcState = SW3_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            SW3_backup.usbState = SW3_CR1_REG;
            SW3_USB_POWER_REG |= SW3_USBIO_ENTER_SLEEP;
            SW3_CR1_REG &= SW3_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SW3__SIO)
        SW3_backup.sioState = SW3_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        SW3_SIO_REG &= (uint32)(~SW3_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: SW3_Wakeup
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
*  Refer to SW3_Sleep() for an example usage.
*******************************************************************************/
void SW3_Wakeup(void)
{
    #if defined(SW3__PC)
        SW3_PC = SW3_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            SW3_USB_POWER_REG &= SW3_USBIO_EXIT_SLEEP_PH1;
            SW3_CR1_REG = SW3_backup.usbState;
            SW3_USB_POWER_REG &= SW3_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SW3__SIO)
        SW3_SIO_REG = SW3_backup.sioState;
    #endif
}


/* [] END OF FILE */
