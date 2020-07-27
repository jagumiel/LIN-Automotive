/*******************************************************************************
* File Name: LIN_1_IntClk.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_LIN_1_IntClk_H)
#define CY_CLOCK_LIN_1_IntClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void LIN_1_IntClk_StartEx(uint32 alignClkDiv);
#define LIN_1_IntClk_Start() \
    LIN_1_IntClk_StartEx(LIN_1_IntClk__PA_DIV_ID)

#else

void LIN_1_IntClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void LIN_1_IntClk_Stop(void);

void LIN_1_IntClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 LIN_1_IntClk_GetDividerRegister(void);
uint8  LIN_1_IntClk_GetFractionalDividerRegister(void);

#define LIN_1_IntClk_Enable()                         LIN_1_IntClk_Start()
#define LIN_1_IntClk_Disable()                        LIN_1_IntClk_Stop()
#define LIN_1_IntClk_SetDividerRegister(clkDivider, reset)  \
    LIN_1_IntClk_SetFractionalDividerRegister((clkDivider), 0u)
#define LIN_1_IntClk_SetDivider(clkDivider)           LIN_1_IntClk_SetDividerRegister((clkDivider), 1u)
#define LIN_1_IntClk_SetDividerValue(clkDivider)      LIN_1_IntClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define LIN_1_IntClk_DIV_ID     LIN_1_IntClk__DIV_ID

#define LIN_1_IntClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define LIN_1_IntClk_CTRL_REG   (*(reg32 *)LIN_1_IntClk__CTRL_REGISTER)
#define LIN_1_IntClk_DIV_REG    (*(reg32 *)LIN_1_IntClk__DIV_REGISTER)

#define LIN_1_IntClk_CMD_DIV_SHIFT          (0u)
#define LIN_1_IntClk_CMD_PA_DIV_SHIFT       (8u)
#define LIN_1_IntClk_CMD_DISABLE_SHIFT      (30u)
#define LIN_1_IntClk_CMD_ENABLE_SHIFT       (31u)

#define LIN_1_IntClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << LIN_1_IntClk_CMD_DISABLE_SHIFT))
#define LIN_1_IntClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << LIN_1_IntClk_CMD_ENABLE_SHIFT))

#define LIN_1_IntClk_DIV_FRAC_MASK  (0x000000F8u)
#define LIN_1_IntClk_DIV_FRAC_SHIFT (3u)
#define LIN_1_IntClk_DIV_INT_MASK   (0xFFFFFF00u)
#define LIN_1_IntClk_DIV_INT_SHIFT  (8u)

#else 

#define LIN_1_IntClk_DIV_REG        (*(reg32 *)LIN_1_IntClk__REGISTER)
#define LIN_1_IntClk_ENABLE_REG     LIN_1_IntClk_DIV_REG
#define LIN_1_IntClk_DIV_FRAC_MASK  LIN_1_IntClk__FRAC_MASK
#define LIN_1_IntClk_DIV_FRAC_SHIFT (16u)
#define LIN_1_IntClk_DIV_INT_MASK   LIN_1_IntClk__DIVIDER_MASK
#define LIN_1_IntClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_LIN_1_IntClk_H) */

/* [] END OF FILE */
