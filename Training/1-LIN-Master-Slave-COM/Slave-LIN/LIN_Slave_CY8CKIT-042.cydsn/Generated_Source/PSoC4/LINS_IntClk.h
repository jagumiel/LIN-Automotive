/*******************************************************************************
* File Name: LINS_IntClk.h
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

#if !defined(CY_CLOCK_LINS_IntClk_H)
#define CY_CLOCK_LINS_IntClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void LINS_IntClk_StartEx(uint32 alignClkDiv);
#define LINS_IntClk_Start() \
    LINS_IntClk_StartEx(LINS_IntClk__PA_DIV_ID)

#else

void LINS_IntClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void LINS_IntClk_Stop(void);

void LINS_IntClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 LINS_IntClk_GetDividerRegister(void);
uint8  LINS_IntClk_GetFractionalDividerRegister(void);

#define LINS_IntClk_Enable()                         LINS_IntClk_Start()
#define LINS_IntClk_Disable()                        LINS_IntClk_Stop()
#define LINS_IntClk_SetDividerRegister(clkDivider, reset)  \
    LINS_IntClk_SetFractionalDividerRegister((clkDivider), 0u)
#define LINS_IntClk_SetDivider(clkDivider)           LINS_IntClk_SetDividerRegister((clkDivider), 1u)
#define LINS_IntClk_SetDividerValue(clkDivider)      LINS_IntClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define LINS_IntClk_DIV_ID     LINS_IntClk__DIV_ID

#define LINS_IntClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define LINS_IntClk_CTRL_REG   (*(reg32 *)LINS_IntClk__CTRL_REGISTER)
#define LINS_IntClk_DIV_REG    (*(reg32 *)LINS_IntClk__DIV_REGISTER)

#define LINS_IntClk_CMD_DIV_SHIFT          (0u)
#define LINS_IntClk_CMD_PA_DIV_SHIFT       (8u)
#define LINS_IntClk_CMD_DISABLE_SHIFT      (30u)
#define LINS_IntClk_CMD_ENABLE_SHIFT       (31u)

#define LINS_IntClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << LINS_IntClk_CMD_DISABLE_SHIFT))
#define LINS_IntClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << LINS_IntClk_CMD_ENABLE_SHIFT))

#define LINS_IntClk_DIV_FRAC_MASK  (0x000000F8u)
#define LINS_IntClk_DIV_FRAC_SHIFT (3u)
#define LINS_IntClk_DIV_INT_MASK   (0xFFFFFF00u)
#define LINS_IntClk_DIV_INT_SHIFT  (8u)

#else 

#define LINS_IntClk_DIV_REG        (*(reg32 *)LINS_IntClk__REGISTER)
#define LINS_IntClk_ENABLE_REG     LINS_IntClk_DIV_REG
#define LINS_IntClk_DIV_FRAC_MASK  LINS_IntClk__FRAC_MASK
#define LINS_IntClk_DIV_FRAC_SHIFT (16u)
#define LINS_IntClk_DIV_INT_MASK   LINS_IntClk__DIVIDER_MASK
#define LINS_IntClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_LINS_IntClk_H) */

/* [] END OF FILE */
