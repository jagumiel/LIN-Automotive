/*****************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
* ============
* This is a demo with the porpouse of receiving frames to set a RGB LED. 
* There are 2 different input frames with multiple signals.
* 
* The LIN Master can read the status of the Lights by sending the Frame ID 0x11
*
* The master receives the following status from LIN Slave based on the lights 
* status as follows:
*
*
*******************************************************************************/

#include "header.h"

/*******************************************************************************
* Function Name: main()
********************************************************************************
*
* Summary:
*  Initialize LIN slave. If the LIN slave receives an unconditional frame from  
*  master with Frame ID 0x01 or 0x10, then the first byte of data (command to control  
*  the lights) is written to the other unconditional frame (OutFrame). Based on 
*  the received command slave will control the lights status.
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
    uint8   dataReceived = 0u;
    uint8   colorValue = 0u;
    uint8_t red, green, blue = 0;
    
	/* Enable Global Interrupts */
    CyGlobalIntEnable;      
    
    /* Call initialization function */
    l_sys_init();
    
    /* Start LINS component */
    if(0u != l_ifc_init(LINS_IFC_HANDLE)){
        /* Switch ON the Red LED */
        //LedRed_Write(1);
    }
    
    /* Start PWM components */
    PWM_R_Start();
    PWM_G_Start();
    PWM_B_Start();
    PWM_R_WriteCompare(0);
    PWM_G_WriteCompare(0);
    PWM_B_WriteCompare(0);
    
	while(1){
    	/*******************************************************
    	* Check if "InFrame" frame is received from LIN Master *
    	*******************************************************/
    	if (1u == l_flg_tst_InFrame()){           
    		/* Clear signal flag */
            l_flg_clr_InFrame();
    		    					
    		/* Copy the value in Insig scalar into OutSig scalar using an
             * intermediate variable (dataReceived) */
            dataReceived = l_u8_rd_RedValue();
    		l_u8_wr(OutSig_SIGNAL_HANDLE, dataReceived);
    		red     = l_u8_rd_RedValue();
            green   = l_u8_rd_GreenValue();
            blue    = l_u8_rd_BlueValue();
    		
            
            PWM_R_WriteCompare(red);
            PWM_G_WriteCompare(green);
            PWM_B_WriteCompare(blue);            
                
        	/* Copy the status of the LED to Output Signals */
            l_u8_wr_OutRedValue(red); 
            l_u8_wr_OutGreenValue(green);
            l_u8_wr_OutBlueValue(blue);
        }
        
        /************************************************************
    	* Check if "InSingleData" frame is received from LIN Master *
    	*************************************************************/
        if (1u == l_flg_tst_InSingleData()){           
    		/* Clear signal flag */
            l_flg_clr_InSingleData();
    		    					
    		/* Copy the value in Insig scalar into OutSig scalar using an
             * intermediate variable (dataReceived) */
            dataReceived = l_u8_rd_Color();
    		l_u8_wr(OutSig_SIGNAL_HANDLE, dataReceived);
            colorValue=l_u8_rd_ColorValue();
            if(dataReceived==0x00){
                red = colorValue;
                PWM_R_WriteCompare(red);
                l_u8_wr_OutRedValue(red); 
            }else if(dataReceived==0x01){
                green = colorValue;
                PWM_G_WriteCompare(green);
                l_u8_wr_OutGreenValue(green);
            }else if(dataReceived==0x10){
                blue = colorValue;
                PWM_B_WriteCompare(blue);
                l_u8_wr_OutBlueValue(blue);
            }
        }
        
    	/***************************************************************
    	* Check if the data in "OutFrame" frame is sent to LIN Master
    	***************************************************************/    		
    	if (l_flg_tst_OutFrame()){
    		/* Clear signal flag */
    		l_flg_clr_OutFrame();
    	}
	}
}


/* [] END OF FILE */