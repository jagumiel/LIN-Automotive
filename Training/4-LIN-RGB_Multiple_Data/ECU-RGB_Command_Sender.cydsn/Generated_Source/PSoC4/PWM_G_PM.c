/*******************************************************************************
* File Name: PWM_G_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_G.h"

static PWM_G_backupStruct PWM_G_backup;


/*******************************************************************************
* Function Name: PWM_G_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_G_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_G_SaveConfig(void) 
{

    #if(!PWM_G_UsingFixedFunction)
        #if(!PWM_G_PWMModeIsCenterAligned)
            PWM_G_backup.PWMPeriod = PWM_G_ReadPeriod();
        #endif /* (!PWM_G_PWMModeIsCenterAligned) */
        PWM_G_backup.PWMUdb = PWM_G_ReadCounter();
        #if (PWM_G_UseStatus)
            PWM_G_backup.InterruptMaskValue = PWM_G_STATUS_MASK;
        #endif /* (PWM_G_UseStatus) */

        #if(PWM_G_DeadBandMode == PWM_G__B_PWM__DBM_256_CLOCKS || \
            PWM_G_DeadBandMode == PWM_G__B_PWM__DBM_2_4_CLOCKS)
            PWM_G_backup.PWMdeadBandValue = PWM_G_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_G_KillModeMinTime)
             PWM_G_backup.PWMKillCounterPeriod = PWM_G_ReadKillTime();
        #endif /* (PWM_G_KillModeMinTime) */

        #if(PWM_G_UseControl)
            PWM_G_backup.PWMControlRegister = PWM_G_ReadControlRegister();
        #endif /* (PWM_G_UseControl) */
    #endif  /* (!PWM_G_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_G_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_G_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_G_RestoreConfig(void) 
{
        #if(!PWM_G_UsingFixedFunction)
            #if(!PWM_G_PWMModeIsCenterAligned)
                PWM_G_WritePeriod(PWM_G_backup.PWMPeriod);
            #endif /* (!PWM_G_PWMModeIsCenterAligned) */

            PWM_G_WriteCounter(PWM_G_backup.PWMUdb);

            #if (PWM_G_UseStatus)
                PWM_G_STATUS_MASK = PWM_G_backup.InterruptMaskValue;
            #endif /* (PWM_G_UseStatus) */

            #if(PWM_G_DeadBandMode == PWM_G__B_PWM__DBM_256_CLOCKS || \
                PWM_G_DeadBandMode == PWM_G__B_PWM__DBM_2_4_CLOCKS)
                PWM_G_WriteDeadTime(PWM_G_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_G_KillModeMinTime)
                PWM_G_WriteKillTime(PWM_G_backup.PWMKillCounterPeriod);
            #endif /* (PWM_G_KillModeMinTime) */

            #if(PWM_G_UseControl)
                PWM_G_WriteControlRegister(PWM_G_backup.PWMControlRegister);
            #endif /* (PWM_G_UseControl) */
        #endif  /* (!PWM_G_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_G_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_G_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_G_Sleep(void) 
{
    #if(PWM_G_UseControl)
        if(PWM_G_CTRL_ENABLE == (PWM_G_CONTROL & PWM_G_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_G_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_G_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_G_UseControl) */

    /* Stop component */
    PWM_G_Stop();

    /* Save registers configuration */
    PWM_G_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_G_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_G_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_G_Wakeup(void) 
{
     /* Restore registers values */
    PWM_G_RestoreConfig();

    if(PWM_G_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_G_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
