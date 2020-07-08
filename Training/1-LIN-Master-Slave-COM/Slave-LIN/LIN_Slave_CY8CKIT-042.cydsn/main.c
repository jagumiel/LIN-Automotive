/*****************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:

* This project demonstrates LIN Slave functionality of PSoC 4. If it (LIN slave) 
* receives an unconditional frame from LIN master with Frame ID 0x10 (InFrame),
* then the first byte of data (command) is written to another unconditional 
* frame (OutFrame). This data command controls the RGB LED color as follows:
*
* Command 0x00 --> All LEDs OFF
* Command 0x11 --> Red LED ON
* Command 0x22 --> Green LED ON
* Command 0x33 --> Blue LED ON
* 
* The LIN Master can read the status of the RGB LED by sending the Frame ID 0x11
*
* The master receives the following status from LIN Slave based on the LED 
* status as follows:
* If Red LED ON   --> 0xAA
* If Green LED ON --> 0xBB
* If Blue LED ON  --> 0xCC
* If All LEDs OFF --> 0xDD
*
* Because the LIN Master component is not available with PSoC 4, a LIN Analyzer  
* is required to test this project
* 
*
* Hardware Dependency:
* This project requires CY8CKIT-026 CAN and LIN Shield Kit, CY8CKIT-042 Pioneer 
* Kit and a third party LIN analyzer
*
* This project needs to be programmed to PSoC 4 of CY8CKIT-042 Pioneer Kit.
*
* CY8CKIT-026 CAN and LIN Shield Kit should be placed on to the Arduino headers 
* of the KIT-042/044. And connect the KIT-026 pins as follows:
* J3_10 to LIN1_RX (J15_1) or LIN2_RX (J6_1)
* J3_9 to LIN1_TX (J15_2) or LIN2_TX (J6_2) 
* A0 (J2_1) to LIN1_NSLP (J15_3) or LIN2_NSLP (J6_3)
* And connect a 12V supply to the 'Vin' pin (J11 or J12) on CY8CKIT-026.
*
* Code Tested With:
* 1. PSoC Creator 3.3 SP1
* 2. Toolchain: ARM GCC 4.9.3
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
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*****************************************************************************/

#include "header.h"

/*******************************************************************************
* Function Name: main()
********************************************************************************
*
* Summary:
*  Initialize LIN slave. If the LIN slave receives an unconditional frame from  
*  master with Frame ID 0x10, then the first byte of data (command to control  
*  the LED) is written to the other unconditional frame (OutFrame). Based on 
*  the received command slave will control the RGB LED color.
*
*  The LIN Master can read the status of the LED by sending the Frame ID 0x11
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
int main(void)
{        
    /* Local Variables */
    uint8 dataArray =  0u ;
    uint8 dataReceived = 0u;
    
	/* Enable Global Interrupts */
    CyGlobalIntEnable;      
    
    /* Call initialization function */
    l_sys_init();
    
    /* Start LINS component */
    if(0u != l_ifc_init(LINS_IFC_HANDLE))
    {
        /* Switch ON the Red LED */
        RGB_LED_ON_RED;
    }
    //RGB_LED_ON_RED;
	while(1)
	{
    	/**********************************************************
    	* Check if "InFrame" frame is received from LIN Master
    	**********************************************************/
    	if (1u == l_flg_tst_InFrame())
    	{           
    		/* Clear signal flag */
            l_flg_clr_InFrame();
    		    					
    		/* Copy the value in Insig scalar into OutSig scalar using an
             * intermediate variable (dataReceived) */
    		dataReceived = l_u8_rd_InSig();
    		l_u8_wr(OutSig_SIGNAL_HANDLE, dataReceived);
            /* Scan the dataReceived and switch on the LED as per the command */
            if(CMD_SET_RED == dataReceived)
            {
                /* Switch ON the Red LED */
                RGB_LED_ON_RED;
                
                /* Store the LED status (RED LED is ON = 0xAA)in dataArray */
                dataArray = CMD_SENT_RED; 
            }
            else if(CMD_SET_GREEN == dataReceived)
            {
                /* Switch ON the Green LED */
                RGB_LED_ON_GREEN;
                
                /* Store the LED status (GREEN LED is ON = 0xBB)in dataArray */
                dataArray = CMD_SENT_GREEN; 
            }
            else if(CMD_SET_BLUE == dataReceived)
            {
                /* Switch ON the Blue LED */
                RGB_LED_ON_BLUE;
                
                /* Store the LED status (BLUE LED is ON = 0xBB)in dataArray */
                dataArray = CMD_SENT_BLUE;                    
            }
            else if(CMD_SET_OFF == dataReceived)
            {
                /* Switch OFF all LEDs */
                RGB_LED_OFF;
                
                /* Store the LED status (LED is OFF = 0xDD)in dataArray */
                dataArray = CMD_SENT_OFF;         
            }
                
        	/* Copy the status of the LED (dataArray) to OutArraySig */
            l_u8_wr_OutArraySig(dataArray);                
        }
        
    	/***************************************************************
    	* Check if the data in "OutFrame" frame is sent to LIN Master
    	***************************************************************/    		
    	if (l_flg_tst_OutFrame())
    	{
    		/* Clear signal flag */
    		l_flg_clr_OutFrame();
    	}
	}
}


/* [] END OF FILE */
