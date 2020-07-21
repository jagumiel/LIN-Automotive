/*******************************************************************************
* File Name: Led_Right.c  
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
#include "Led_Right.h"

static Led_Right_BACKUP_STRUCT  Led_Right_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Led_Right_Sleep
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
*  \snippet Led_Right_SUT.c usage_Led_Right_Sleep_Wakeup
*******************************************************************************/
void Led_Right_Sleep(void)
{
    #if defined(Led_Right__PC)
        Led_Right_backup.pcState = Led_Right_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Led_Right_backup.usbState = Led_Right_CR1_REG;
            Led_Right_USB_POWER_REG |= Led_Right_USBIO_ENTER_SLEEP;
            Led_Right_CR1_REG &= Led_Right_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Led_Right__SIO)
        Led_Right_backup.sioState = Led_Right_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Led_Right_SIO_REG &= (uint32)(~Led_Right_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Led_Right_Wakeup
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
*  Refer to Led_Right_Sleep() for an example usage.
*******************************************************************************/
void Led_Right_Wakeup(void)
{
    #if defined(Led_Right__PC)
        Led_Right_PC = Led_Right_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Led_Right_USB_POWER_REG &= Led_Right_USBIO_EXIT_SLEEP_PH1;
            Led_Right_CR1_REG = Led_Right_backup.usbState;
            Led_Right_USB_POWER_REG &= Led_Right_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Led_Right__SIO)
        Led_Right_SIO_REG = Led_Right_backup.sioState;
    #endif
}


/* [] END OF FILE */
