/*******************************************************************************
* File Name: LIN_1_IntClk.c
* Version 2.20
*
*  Description:
*   Provides system API for the clocking, interrupts and watchdog timer.
*
*  Note:
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <cydevice_trm.h>
#include "LIN_1_IntClk.h"

#if defined CYREG_PERI_DIV_CMD

/*******************************************************************************
* Function Name: LIN_1_IntClk_StartEx
********************************************************************************
*
* Summary:
*  Starts the clock, aligned to the specified running clock.
*
* Parameters:
*  alignClkDiv:  The divider to which phase alignment is performed when the
*    clock is started.
*
* Returns:
*  None
*
*******************************************************************************/
void LIN_1_IntClk_StartEx(uint32 alignClkDiv)
{
    /* Make sure any previous start command has finished. */
    while((LIN_1_IntClk_CMD_REG & LIN_1_IntClk_CMD_ENABLE_MASK) != 0u)
    {
    }
    
    /* Specify the target divider and it's alignment divider, and enable. */
    LIN_1_IntClk_CMD_REG =
        ((uint32)LIN_1_IntClk__DIV_ID << LIN_1_IntClk_CMD_DIV_SHIFT)|
        (alignClkDiv << LIN_1_IntClk_CMD_PA_DIV_SHIFT) |
        (uint32)LIN_1_IntClk_CMD_ENABLE_MASK;
}

#else

/*******************************************************************************
* Function Name: LIN_1_IntClk_Start
********************************************************************************
*
* Summary:
*  Starts the clock.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/

void LIN_1_IntClk_Start(void)
{
    /* Set the bit to enable the clock. */
    LIN_1_IntClk_ENABLE_REG |= LIN_1_IntClk__ENABLE_MASK;
}

#endif /* CYREG_PERI_DIV_CMD */


/*******************************************************************************
* Function Name: LIN_1_IntClk_Stop
********************************************************************************
*
* Summary:
*  Stops the clock and returns immediately. This API does not require the
*  source clock to be running but may return before the hardware is actually
*  disabled.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/
void LIN_1_IntClk_Stop(void)
{
#if defined CYREG_PERI_DIV_CMD

    /* Make sure any previous start command has finished. */
    while((LIN_1_IntClk_CMD_REG & LIN_1_IntClk_CMD_ENABLE_MASK) != 0u)
    {
    }
    
    /* Specify the target divider and it's alignment divider, and disable. */
    LIN_1_IntClk_CMD_REG =
        ((uint32)LIN_1_IntClk__DIV_ID << LIN_1_IntClk_CMD_DIV_SHIFT)|
        ((uint32)LIN_1_IntClk_CMD_DISABLE_MASK);

#else

    /* Clear the bit to disable the clock. */
    LIN_1_IntClk_ENABLE_REG &= (uint32)(~LIN_1_IntClk__ENABLE_MASK);
    
#endif /* CYREG_PERI_DIV_CMD */
}


/*******************************************************************************
* Function Name: LIN_1_IntClk_SetFractionalDividerRegister
********************************************************************************
*
* Summary:
*  Modifies the clock divider and the fractional divider.
*
* Parameters:
*  clkDivider:  Divider register value (0-65535). This value is NOT the
*    divider; the clock hardware divides by clkDivider plus one. For example,
*    to divide the clock by 2, this parameter should be set to 1.
*  fracDivider:  Fractional Divider register value (0-31).
* Returns:
*  None
*
*******************************************************************************/
void LIN_1_IntClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional)
{
    uint32 maskVal;
    uint32 regVal;
    
#if defined (LIN_1_IntClk__FRAC_MASK) || defined (CYREG_PERI_DIV_CMD)
    
	/* get all but divider bits */
    maskVal = LIN_1_IntClk_DIV_REG & 
                    (uint32)(~(uint32)(LIN_1_IntClk_DIV_INT_MASK | LIN_1_IntClk_DIV_FRAC_MASK)); 
	/* combine mask and new divider vals into 32-bit value */
    regVal = maskVal |
        ((uint32)((uint32)clkDivider <<  LIN_1_IntClk_DIV_INT_SHIFT) & LIN_1_IntClk_DIV_INT_MASK) |
        ((uint32)((uint32)clkFractional << LIN_1_IntClk_DIV_FRAC_SHIFT) & LIN_1_IntClk_DIV_FRAC_MASK);
    
#else
    /* get all but integer divider bits */
    maskVal = LIN_1_IntClk_DIV_REG & (uint32)(~(uint32)LIN_1_IntClk__DIVIDER_MASK);
    /* combine mask and new divider val into 32-bit value */
    regVal = clkDivider | maskVal;
    
#endif /* LIN_1_IntClk__FRAC_MASK || CYREG_PERI_DIV_CMD */

    LIN_1_IntClk_DIV_REG = regVal;
}


/*******************************************************************************
* Function Name: LIN_1_IntClk_GetDividerRegister
********************************************************************************
*
* Summary:
*  Gets the clock divider register value.
*
* Parameters:
*  None
*
* Returns:
*  Divide value of the clock minus 1. For example, if the clock is set to
*  divide by 2, the return value will be 1.
*
*******************************************************************************/
uint16 LIN_1_IntClk_GetDividerRegister(void)
{
    return (uint16)((LIN_1_IntClk_DIV_REG & LIN_1_IntClk_DIV_INT_MASK)
        >> LIN_1_IntClk_DIV_INT_SHIFT);
}


/*******************************************************************************
* Function Name: LIN_1_IntClk_GetFractionalDividerRegister
********************************************************************************
*
* Summary:
*  Gets the clock fractional divider register value.
*
* Parameters:
*  None
*
* Returns:
*  Fractional Divide value of the clock
*  0 if the fractional divider is not in use.
*
*******************************************************************************/
uint8 LIN_1_IntClk_GetFractionalDividerRegister(void)
{
#if defined (LIN_1_IntClk__FRAC_MASK)
    /* return fractional divider bits */
    return (uint8)((LIN_1_IntClk_DIV_REG & LIN_1_IntClk_DIV_FRAC_MASK)
        >> LIN_1_IntClk_DIV_FRAC_SHIFT);
#else
    return 0u;
#endif /* LIN_1_IntClk__FRAC_MASK */
}


/* [] END OF FILE */
