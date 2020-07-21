/*******************************************************************************
* File Name: PWM_Left_PM.c
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

#include "PWM_Left.h"

static PWM_Left_backupStruct PWM_Left_backup;


/*******************************************************************************
* Function Name: PWM_Left_SaveConfig
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
*  PWM_Left_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_Left_SaveConfig(void) 
{

    #if(!PWM_Left_UsingFixedFunction)
        #if(!PWM_Left_PWMModeIsCenterAligned)
            PWM_Left_backup.PWMPeriod = PWM_Left_ReadPeriod();
        #endif /* (!PWM_Left_PWMModeIsCenterAligned) */
        PWM_Left_backup.PWMUdb = PWM_Left_ReadCounter();
        #if (PWM_Left_UseStatus)
            PWM_Left_backup.InterruptMaskValue = PWM_Left_STATUS_MASK;
        #endif /* (PWM_Left_UseStatus) */

        #if(PWM_Left_DeadBandMode == PWM_Left__B_PWM__DBM_256_CLOCKS || \
            PWM_Left_DeadBandMode == PWM_Left__B_PWM__DBM_2_4_CLOCKS)
            PWM_Left_backup.PWMdeadBandValue = PWM_Left_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_Left_KillModeMinTime)
             PWM_Left_backup.PWMKillCounterPeriod = PWM_Left_ReadKillTime();
        #endif /* (PWM_Left_KillModeMinTime) */

        #if(PWM_Left_UseControl)
            PWM_Left_backup.PWMControlRegister = PWM_Left_ReadControlRegister();
        #endif /* (PWM_Left_UseControl) */
    #endif  /* (!PWM_Left_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_Left_RestoreConfig
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
*  PWM_Left_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Left_RestoreConfig(void) 
{
        #if(!PWM_Left_UsingFixedFunction)
            #if(!PWM_Left_PWMModeIsCenterAligned)
                PWM_Left_WritePeriod(PWM_Left_backup.PWMPeriod);
            #endif /* (!PWM_Left_PWMModeIsCenterAligned) */

            PWM_Left_WriteCounter(PWM_Left_backup.PWMUdb);

            #if (PWM_Left_UseStatus)
                PWM_Left_STATUS_MASK = PWM_Left_backup.InterruptMaskValue;
            #endif /* (PWM_Left_UseStatus) */

            #if(PWM_Left_DeadBandMode == PWM_Left__B_PWM__DBM_256_CLOCKS || \
                PWM_Left_DeadBandMode == PWM_Left__B_PWM__DBM_2_4_CLOCKS)
                PWM_Left_WriteDeadTime(PWM_Left_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_Left_KillModeMinTime)
                PWM_Left_WriteKillTime(PWM_Left_backup.PWMKillCounterPeriod);
            #endif /* (PWM_Left_KillModeMinTime) */

            #if(PWM_Left_UseControl)
                PWM_Left_WriteControlRegister(PWM_Left_backup.PWMControlRegister);
            #endif /* (PWM_Left_UseControl) */
        #endif  /* (!PWM_Left_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_Left_Sleep
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
*  PWM_Left_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_Left_Sleep(void) 
{
    #if(PWM_Left_UseControl)
        if(PWM_Left_CTRL_ENABLE == (PWM_Left_CONTROL & PWM_Left_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_Left_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_Left_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_Left_UseControl) */

    /* Stop component */
    PWM_Left_Stop();

    /* Save registers configuration */
    PWM_Left_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Left_Wakeup
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
*  PWM_Left_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Left_Wakeup(void) 
{
     /* Restore registers values */
    PWM_Left_RestoreConfig();

    if(PWM_Left_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_Left_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
