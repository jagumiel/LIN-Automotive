/*******************************************************************************
* File Name: header.h
*
* Version: 1.0
*
* Description:
*  This file provides function prototypes, constants and macros for the example
*  project.
*
********************************************************************************
* Copyright 2015-2016, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>


/***************************************
*            Constants
****************************************/


/* Commands set */
#define CMD_TURN_ON_LEFT    (0x01u)
#define CMD_TURN_OFF_LEFT   (0x00u)
#define CMD_TURN_ON_RIGHT   (0x11u)
#define CMD_TURN_OFF_RIGHT  (0x10u)

/* Commands sent back to master*/
#define CMD_SENT_LEFT_ON    (0xA1u)
#define CMD_SENT_LEFT_OFF   (0xA0u)
#define CMD_SENT_RIGHT_ON   (0xB1u)
#define CMD_SENT_RIGHT_OFF  (0xB0u)

/* [] END OF FILE */
