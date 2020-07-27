/*******************************************************************************
* File Name: PWM_B_PM.c
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

#include "PWM_B.h"

static PWM_B_backupStruct PWM_B_backup;


/*******************************************************************************
* Function Name: PWM_B_SaveConfig
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
*  PWM_B_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_B_SaveConfig(void) 
{

    #if(!PWM_B_UsingFixedFunction)
        #if(!PWM_B_PWMModeIsCenterAligned)
            PWM_B_backup.PWMPeriod = PWM_B_ReadPeriod();
        #endif /* (!PWM_B_PWMModeIsCenterAligned) */
        PWM_B_backup.PWMUdb = PWM_B_ReadCounter();
        #if (PWM_B_UseStatus)
            PWM_B_backup.InterruptMaskValue = PWM_B_STATUS_MASK;
        #endif /* (PWM_B_UseStatus) */

        #if(PWM_B_DeadBandMode == PWM_B__B_PWM__DBM_256_CLOCKS || \
            PWM_B_DeadBandMode == PWM_B__B_PWM__DBM_2_4_CLOCKS)
            PWM_B_backup.PWMdeadBandValue = PWM_B_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_B_KillModeMinTime)
             PWM_B_backup.PWMKillCounterPeriod = PWM_B_ReadKillTime();
        #endif /* (PWM_B_KillModeMinTime) */

        #if(PWM_B_UseControl)
            PWM_B_backup.PWMControlRegister = PWM_B_ReadControlRegister();
        #endif /* (PWM_B_UseControl) */
    #endif  /* (!PWM_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_B_RestoreConfig
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
*  PWM_B_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_B_RestoreConfig(void) 
{
        #if(!PWM_B_UsingFixedFunction)
            #if(!PWM_B_PWMModeIsCenterAligned)
                PWM_B_WritePeriod(PWM_B_backup.PWMPeriod);
            #endif /* (!PWM_B_PWMModeIsCenterAligned) */

            PWM_B_WriteCounter(PWM_B_backup.PWMUdb);

            #if (PWM_B_UseStatus)
                PWM_B_STATUS_MASK = PWM_B_backup.InterruptMaskValue;
            #endif /* (PWM_B_UseStatus) */

            #if(PWM_B_DeadBandMode == PWM_B__B_PWM__DBM_256_CLOCKS || \
                PWM_B_DeadBandMode == PWM_B__B_PWM__DBM_2_4_CLOCKS)
                PWM_B_WriteDeadTime(PWM_B_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_B_KillModeMinTime)
                PWM_B_WriteKillTime(PWM_B_backup.PWMKillCounterPeriod);
            #endif /* (PWM_B_KillModeMinTime) */

            #if(PWM_B_UseControl)
                PWM_B_WriteControlRegister(PWM_B_backup.PWMControlRegister);
            #endif /* (PWM_B_UseControl) */
        #endif  /* (!PWM_B_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_B_Sleep
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
*  PWM_B_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_B_Sleep(void) 
{
    #if(PWM_B_UseControl)
        if(PWM_B_CTRL_ENABLE == (PWM_B_CONTROL & PWM_B_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_B_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_B_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_B_UseControl) */

    /* Stop component */
    PWM_B_Stop();

    /* Save registers configuration */
    PWM_B_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_B_Wakeup
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
*  PWM_B_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_B_Wakeup(void) 
{
     /* Restore registers values */
    PWM_B_RestoreConfig();

    if(PWM_B_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_B_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
