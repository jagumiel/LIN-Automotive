/*******************************************************************************
* File Name: LedBlue.h  
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

#if !defined(CY_PINS_LedBlue_ALIASES_H) /* Pins LedBlue_ALIASES_H */
#define CY_PINS_LedBlue_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define LedBlue_0			(LedBlue__0__PC)
#define LedBlue_0_PS		(LedBlue__0__PS)
#define LedBlue_0_PC		(LedBlue__0__PC)
#define LedBlue_0_DR		(LedBlue__0__DR)
#define LedBlue_0_SHIFT	(LedBlue__0__SHIFT)
#define LedBlue_0_INTR	((uint16)((uint16)0x0003u << (LedBlue__0__SHIFT*2u)))

#define LedBlue_INTR_ALL	 ((uint16)(LedBlue_0_INTR))


#endif /* End Pins LedBlue_ALIASES_H */


/* [] END OF FILE */
