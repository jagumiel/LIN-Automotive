/*******************************************************************************
* File Name: PWM_R_PM.c
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

#include "PWM_R.h"

static PWM_R_backupStruct PWM_R_backup;


/*******************************************************************************
* Function Name: PWM_R_SaveConfig
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
*  PWM_R_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_R_SaveConfig(void) 
{

    #if(!PWM_R_UsingFixedFunction)
        #if(!PWM_R_PWMModeIsCenterAligned)
            PWM_R_backup.PWMPeriod = PWM_R_ReadPeriod();
        #endif /* (!PWM_R_PWMModeIsCenterAligned) */
        PWM_R_backup.PWMUdb = PWM_R_ReadCounter();
        #if (PWM_R_UseStatus)
            PWM_R_backup.InterruptMaskValue = PWM_R_STATUS_MASK;
        #endif /* (PWM_R_UseStatus) */

        #if(PWM_R_DeadBandMode == PWM_R__B_PWM__DBM_256_CLOCKS || \
            PWM_R_DeadBandMode == PWM_R__B_PWM__DBM_2_4_CLOCKS)
            PWM_R_backup.PWMdeadBandValue = PWM_R_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_R_KillModeMinTime)
             PWM_R_backup.PWMKillCounterPeriod = PWM_R_ReadKillTime();
        #endif /* (PWM_R_KillModeMinTime) */

        #if(PWM_R_UseControl)
            PWM_R_backup.PWMControlRegister = PWM_R_ReadControlRegister();
        #endif /* (PWM_R_UseControl) */
    #endif  /* (!PWM_R_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_R_RestoreConfig
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
*  PWM_R_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_R_RestoreConfig(void) 
{
        #if(!PWM_R_UsingFixedFunction)
            #if(!PWM_R_PWMModeIsCenterAligned)
                PWM_R_WritePeriod(PWM_R_backup.PWMPeriod);
            #endif /* (!PWM_R_PWMModeIsCenterAligned) */

            PWM_R_WriteCounter(PWM_R_backup.PWMUdb);

            #if (PWM_R_UseStatus)
                PWM_R_STATUS_MASK = PWM_R_backup.InterruptMaskValue;
            #endif /* (PWM_R_UseStatus) */

            #if(PWM_R_DeadBandMode == PWM_R__B_PWM__DBM_256_CLOCKS || \
                PWM_R_DeadBandMode == PWM_R__B_PWM__DBM_2_4_CLOCKS)
                PWM_R_WriteDeadTime(PWM_R_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_R_KillModeMinTime)
                PWM_R_WriteKillTime(PWM_R_backup.PWMKillCounterPeriod);
            #endif /* (PWM_R_KillModeMinTime) */

            #if(PWM_R_UseControl)
                PWM_R_WriteControlRegister(PWM_R_backup.PWMControlRegister);
            #endif /* (PWM_R_UseControl) */
        #endif  /* (!PWM_R_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_R_Sleep
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
*  PWM_R_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_R_Sleep(void) 
{
    #if(PWM_R_UseControl)
        if(PWM_R_CTRL_ENABLE == (PWM_R_CONTROL & PWM_R_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_R_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_R_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_R_UseControl) */

    /* Stop component */
    PWM_R_Stop();

    /* Save registers configuration */
    PWM_R_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_R_Wakeup
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
*  PWM_R_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_R_Wakeup(void) 
{
     /* Restore registers values */
    PWM_R_RestoreConfig();

    if(PWM_R_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_R_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
