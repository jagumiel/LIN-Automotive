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
*******************************************************************************/

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
    
    /* Start PWM components */
    PWM_Left_Start();
    PWM_Right_Start();
    
	/* Enable Global Interrupts */
    CyGlobalIntEnable;      
    
    /* Call initialization function */
    l_sys_init();
    
    /* Start LINS component */
    if(0u != l_ifc_init(LINS_IFC_HANDLE))
    {
        /* Switch ON the Red LED */
        RedLed_Write(1);
    }
    
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
            if(CMD_TURN_ON_LEFT == dataReceived){
                /* Turn on left led */
                for(int i=255; i>2; i--){
                    PWM_Left_WriteCompare(i);
                    CyDelay(5);
                }
                
                /* Store the LED status (RED LED is ON = 0xAA)in dataArray */
                dataArray = CMD_SENT_LEFT_ON; 
            }else if(CMD_TURN_OFF_LEFT == dataReceived){
                /* Turn off left led */
                for(int i=1; i<256; i++){
                    PWM_Left_WriteCompare(i);
                    CyDelay(5);
                }
                
                /* Store the LED status (GREEN LED is ON = 0xBB)in dataArray */
                dataArray = CMD_SENT_LEFT_OFF; 
            }else if(CMD_TURN_ON_RIGHT == dataReceived){
                /* Turn on right led */
                for(int i=255; i>2; i--){
                    PWM_Right_WriteCompare(i);
                    CyDelay(5);
                }
                
                /* Store the LED status (BLUE LED is ON = 0xBB)in dataArray */
                dataArray = CMD_SENT_RIGHT_ON;                    
            }else if(CMD_TURN_OFF_RIGHT == dataReceived){
                /* Turn off right led */
                for(int i=1; i<256; i++){
                    PWM_Right_WriteCompare(i);
                    CyDelay(5);
                }
                
                /* Store the LED status (LED is OFF = 0xDD)in dataArray */
                dataArray = CMD_SENT_RIGHT_OFF;         
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
