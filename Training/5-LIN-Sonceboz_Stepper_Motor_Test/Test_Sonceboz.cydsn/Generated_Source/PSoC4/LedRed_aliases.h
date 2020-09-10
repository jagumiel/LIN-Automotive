/*******************************************************************************
* File Name: LedRed.h  
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

#if !defined(CY_PINS_LedRed_ALIASES_H) /* Pins LedRed_ALIASES_H */
#define CY_PINS_LedRed_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define LedRed_0			(LedRed__0__PC)
#define LedRed_0_PS		(LedRed__0__PS)
#define LedRed_0_PC		(LedRed__0__PC)
#define LedRed_0_DR		(LedRed__0__DR)
#define LedRed_0_SHIFT	(LedRed__0__SHIFT)
#define LedRed_0_INTR	((uint16)((uint16)0x0003u << (LedRed__0__SHIFT*2u)))

#define LedRed_INTR_ALL	 ((uint16)(LedRed_0_INTR))


#endif /* End Pins LedRed_ALIASES_H */


/* [] END OF FILE */
