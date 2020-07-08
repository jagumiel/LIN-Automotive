/*******************************************************************************
* File Name: WakeUp.h  
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

#if !defined(CY_PINS_WakeUp_ALIASES_H) /* Pins WakeUp_ALIASES_H */
#define CY_PINS_WakeUp_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define WakeUp_0			(WakeUp__0__PC)
#define WakeUp_0_PS		(WakeUp__0__PS)
#define WakeUp_0_PC		(WakeUp__0__PC)
#define WakeUp_0_DR		(WakeUp__0__DR)
#define WakeUp_0_SHIFT	(WakeUp__0__SHIFT)
#define WakeUp_0_INTR	((uint16)((uint16)0x0003u << (WakeUp__0__SHIFT*2u)))

#define WakeUp_INTR_ALL	 ((uint16)(WakeUp_0_INTR))


#endif /* End Pins WakeUp_ALIASES_H */


/* [] END OF FILE */
