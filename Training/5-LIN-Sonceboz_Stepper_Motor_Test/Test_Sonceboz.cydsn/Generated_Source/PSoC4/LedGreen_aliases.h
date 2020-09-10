/*******************************************************************************
* File Name: LedGreen.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_LedGreen_ALIASES_H) /* Pins LedGreen_ALIASES_H */
#define CY_PINS_LedGreen_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define LedGreen_0			(LedGreen__0__PC)
#define LedGreen_0_PS		(LedGreen__0__PS)
#define LedGreen_0_PC		(LedGreen__0__PC)
#define LedGreen_0_DR		(LedGreen__0__DR)
#define LedGreen_0_SHIFT	(LedGreen__0__SHIFT)
#define LedGreen_0_INTR	((uint16)((uint16)0x0003u << (LedGreen__0__SHIFT*2u)))

#define LedGreen_INTR_ALL	 ((uint16)(LedGreen_0_INTR))


#endif /* End Pins LedGreen_ALIASES_H */


/* [] END OF FILE */
