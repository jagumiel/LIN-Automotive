/*******************************************************************************
* File Name: NSLP.c  
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
#include "NSLP.h"

static NSLP_BACKUP_STRUCT  NSLP_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: NSLP_Sleep
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
*  \snippet NSLP_SUT.c usage_NSLP_Sleep_Wakeup
*******************************************************************************/
void NSLP_Sleep(void)
{
    #if defined(NSLP__PC)
        NSLP_backup.pcState = NSLP_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            NSLP_backup.usbState = NSLP_CR1_REG;
            NSLP_USB_POWER_REG |= NSLP_USBIO_ENTER_SLEEP;
            NSLP_CR1_REG &= NSLP_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(NSLP__SIO)
        NSLP_backup.sioState = NSLP_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        NSLP_SIO_REG &= (uint32)(~NSLP_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: NSLP_Wakeup
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
*  Refer to NSLP_Sleep() for an example usage.
*******************************************************************************/
void NSLP_Wakeup(void)
{
    #if defined(NSLP__PC)
        NSLP_PC = NSLP_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            NSLP_USB_POWER_REG &= NSLP_USBIO_EXIT_SLEEP_PH1;
            NSLP_CR1_REG = NSLP_backup.usbState;
            NSLP_USB_POWER_REG &= NSLP_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(NSLP__SIO)
        NSLP_SIO_REG = NSLP_backup.sioState;
    #endif
}


/* [] END OF FILE */
