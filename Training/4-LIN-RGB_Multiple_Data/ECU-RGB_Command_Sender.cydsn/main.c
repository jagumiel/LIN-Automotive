/* ===========================================
 * ___________________________________________
 *          LIN: RGB Command Sender
 * -------------------------------------------
 * In this project a command will be readed
 * from a PC using UART communication. User
 * will introduce the desired values for the
 * RGB LEDs. The Master will analyse the data
 * and will send it to the LIN Slave. In this
 * example 2 different kind of frames will be
 * sent to set the LEDs. It is posible to send
 * a frame with 3 bytes, to set the value for
 * all the RGB LED values, or to change the
 * brightness of only one LED using 2 bytes.
 * In the last case, the first byte indicates
 * the colour of the LED to change, and the
 * second byte, the brightness.
 *
 * A LIN transceiver will be needed for
 * this project.
 *
 * Cypress CY8CKIT-042 and CY8CKIT-026 are
 * used in this project.
 *
 * ===========================================
*/

#include <stdlib.h> /* Needed for "atoi()" function */
#include "project.h"
#include "LIN_Frame_Calculations.h"

volatile unsigned char Sensor_data[8];
const uint8_t sync = 0x55;
uint8_t id = 0x10;
uint8_t packet[PACKET_SIZE] = {0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t pid = 0x00;
uint8_t checksum = 0x00;

void getResults(char cmd, char ledValue[]){
    /* Gets the scratch buffer data (string of chars) and places it into color variables (as integers). */
    char singleValAux[3];
    int i=0;
    if(cmd=='W'){
        id=0x10;
        for(int j=0; j<3; j++){
            singleValAux[j]=ledValue[i];
            i++;
        }
        packet[0] = atoi(singleValAux);
        for(int j=0; j<3; j++){
            singleValAux[j]=ledValue[i];
            i++;
        }
        packet[1] = atoi(singleValAux);
        for(int j=0; j<3; j++){
            singleValAux[j]=ledValue[i];
            i++;
        }
        packet[2] = atoi(singleValAux);
        
        /*Numbers must be comprehended between 0-255. If they are greater than 255 are setted to the max value.*/
        if (packet[0] > 255) packet[0] = 255;
        if (packet[1] > 255) packet[1] = 255;
        if (packet[2] > 255) packet[2] = 255;
    }else{
        /* Does exactly the same, but in this case, it only sets 1 value instead of the three RGB values. */
        id=0x01;
        /* Sets the first byte of the frame. It identifies the color. */
        if(cmd=='R'){
            packet[0]=0x00;
        }else if(cmd=='G'){
            packet[0]=0x01;
        }else{
            packet[0]=0x10;
        }
        for(int j=0; j<3; j++){
            singleValAux[j]=ledValue[i];
            i++;
        }
        packet[1] = atoi(singleValAux);
        if (packet[1] > 255) packet[1] = 255;
    }
}

_Bool getData(){
	char charCMD;
	char charData;
	char cop;
	char colorData[9];
	int8 i;
	
	charCMD = UART_PC_UartGetChar();    // this command Gets then clears to 0u
	i = 0;
	
	if (charCMD == 'W' || charCMD == 'R' || charCMD == 'G' || charCMD == 'B'){
		if (charCMD == 'W'){
			cop = 'M';  //Multiple Data
		}else{
			cop = 'S';  //Single Data		
		}
		while (cop != 'X'){
			charData = UART_PC_UartGetChar();
			if ((charData != 0u)){
				/* Only digits will be processed. Any other char will be omitted. */
				if( charData >= '0' && charData <= '9' ){
					colorData[i] = charData;
					i++;
				}
			}
			/* If the array is completed, then the introduced value us printed on screen and the leds are setted. */
			if ((i==9 && cop=='M') ||(i==3 && cop=='S')){
				UART_PC_UartPutString(colorData);
				UART_PC_UartPutChar('\r');
				UART_PC_UartPutChar('\n');
				getResults(charCMD, colorData);
				for(int i=0; i<9; i++)
					colorData[i] = 0u;
				cop = 'X';//X means Undefined. Breaks loop condition.
			}
		}
		return 1;
	}
    return 0;
}

void sendFrame(void){
    UART_MASTER_UartSendBreakBlocking(13);                          // Break
    UART_MASTER_UartPutChar(sync);                                  // Sync (0x55)
    UART_MASTER_UartPutChar(pid);                                   // Protected ID (PID). Se obtiene a partir del ID.
    for(int i=0; i<8; i++){
        UART_MASTER_UartPutChar(packet[i]);                         // Command Frame
    }
    UART_MASTER_UartPutChar(checksum);                              // Checksum. En LIN 2.2 se obtiene a partir del PID y del CMD enviado.*/
}


int main(void){
    
    /* Enable global interrupts. */
    CyGlobalIntEnable;

    /* Start UART/LIN Master */
    UART_MASTER_Start();
    UART_PC_Start();

    /* Turn off the led */
    LedRed_Write(1);
    
    UART_PC_UartPutString("Introduce the values for the RGB colors using this pattern: Wrrrgggbbb.\n\r Example: W255090030\n\r");

    for (;;){
        /* It is not needed to clean the array before filling it again. */
        /*for(int i=0; i<8; i++){
            packet[i]=0xFF;
        }*/
        if(getData()==1){
            pid=LIN_PID_Calculator(id);
            checksum= LIN_Checksum_Calculation(pid, packet);
            sendFrame();
        }
	}
}