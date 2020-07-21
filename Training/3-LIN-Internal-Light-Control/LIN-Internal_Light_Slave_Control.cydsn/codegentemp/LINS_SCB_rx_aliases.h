/*******************************************************************************
* File Name: LINS_SCB_rx.h  
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

#if !defined(CY_PINS_LINS_SCB_rx_ALIASES_H) /* Pins LINS_SCB_rx_ALIASES_H */
#define CY_PINS_LINS_SCB_rx_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define LINS_SCB_rx_0			(LINS_SCB_rx__0__PC)
#define LINS_SCB_rx_0_PS		(LINS_SCB_rx__0__PS)
#define LINS_SCB_rx_0_PC		(LINS_SCB_rx__0__PC)
#define LINS_SCB_rx_0_DR		(LINS_SCB_rx__0__DR)
#define LINS_SCB_rx_0_SHIFT	(LINS_SCB_rx__0__SHIFT)
#define LINS_SCB_rx_0_INTR	((uint16)((uint16)0x0003u << (LINS_SCB_rx__0__SHIFT*2u)))

#define LINS_SCB_rx_INTR_ALL	 ((uint16)(LINS_SCB_rx_0_INTR))


#endif /* End Pins LINS_SCB_rx_ALIASES_H */


/* [] END OF FILE */
