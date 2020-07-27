/*******************************************************************************
* File Name: LIN_1_SCB_rx.h  
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

#if !defined(CY_PINS_LIN_1_SCB_rx_ALIASES_H) /* Pins LIN_1_SCB_rx_ALIASES_H */
#define CY_PINS_LIN_1_SCB_rx_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define LIN_1_SCB_rx_0			(LIN_1_SCB_rx__0__PC)
#define LIN_1_SCB_rx_0_PS		(LIN_1_SCB_rx__0__PS)
#define LIN_1_SCB_rx_0_PC		(LIN_1_SCB_rx__0__PC)
#define LIN_1_SCB_rx_0_DR		(LIN_1_SCB_rx__0__DR)
#define LIN_1_SCB_rx_0_SHIFT	(LIN_1_SCB_rx__0__SHIFT)
#define LIN_1_SCB_rx_0_INTR	((uint16)((uint16)0x0003u << (LIN_1_SCB_rx__0__SHIFT*2u)))

#define LIN_1_SCB_rx_INTR_ALL	 ((uint16)(LIN_1_SCB_rx_0_INTR))


#endif /* End Pins LIN_1_SCB_rx_ALIASES_H */


/* [] END OF FILE */
