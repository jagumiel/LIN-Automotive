/*******************************************************************************
* File Name: RedLed.h  
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

#if !defined(CY_PINS_RedLed_ALIASES_H) /* Pins RedLed_ALIASES_H */
#define CY_PINS_RedLed_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RedLed_0			(RedLed__0__PC)
#define RedLed_0_PS		(RedLed__0__PS)
#define RedLed_0_PC		(RedLed__0__PC)
#define RedLed_0_DR		(RedLed__0__DR)
#define RedLed_0_SHIFT	(RedLed__0__SHIFT)
#define RedLed_0_INTR	((uint16)((uint16)0x0003u << (RedLed__0__SHIFT*2u)))

#define RedLed_INTR_ALL	 ((uint16)(RedLed_0_INTR))


#endif /* End Pins RedLed_ALIASES_H */


/* [] END OF FILE */
