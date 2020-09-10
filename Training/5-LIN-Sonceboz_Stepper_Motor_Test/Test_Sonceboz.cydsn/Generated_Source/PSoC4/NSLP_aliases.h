/*******************************************************************************
* File Name: NSLP.h  
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

#if !defined(CY_PINS_NSLP_ALIASES_H) /* Pins NSLP_ALIASES_H */
#define CY_PINS_NSLP_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define NSLP_0			(NSLP__0__PC)
#define NSLP_0_PS		(NSLP__0__PS)
#define NSLP_0_PC		(NSLP__0__PC)
#define NSLP_0_DR		(NSLP__0__DR)
#define NSLP_0_SHIFT	(NSLP__0__SHIFT)
#define NSLP_0_INTR	((uint16)((uint16)0x0003u << (NSLP__0__SHIFT*2u)))

#define NSLP_INTR_ALL	 ((uint16)(NSLP_0_INTR))


#endif /* End Pins NSLP_ALIASES_H */


/* [] END OF FILE */
