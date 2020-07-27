/* ===========================================
 * ___________________________________________
 *          LIN: RGB Command Sender
 * -------------------------------------------
 * In this project a command will be readed
 * from a PC using UART communication. User
 * will introduce the desired values for the
 * RGB LEDs. The Master will analyse the data
 * and will send it to the LIN Slave. In this
 * example 3 consecutive bytes of data will be
 * sent.
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
uint8_t packet[PACKET_SIZE] = {0x89, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t pid = 0x00;
uint8_t checksum = 0x00;

void getResults(char ledValue[]){
    /* Gets the scratch buffer data (string of chars) and places it into color variables (as integers). */
    char singleValAux[3];
    int i=0;
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
    for(int j=0; j<3; j++){
        singleValAux[j]=ledValue[i];
        i++;
    }
    packet[3] = atoi(singleValAux);
    
    /*Numbers must be comprehended between 0-255. If they are greater than 255 are setted to the max value.*/
    if (packet[1] > 255) packet[0] = 255;
    if (packet[2] > 255) packet[1] = 255;
    if (packet[3] > 255) packet[2] = 255;
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
    
    char ch;
    char ch1;
    char cop;
    char buffer[9];
    int8 tic;
    
    /* Enable global interrupts. */
    CyGlobalIntEnable;

    /* Start UART/LIN Master */
    UART_MASTER_Start();
    UART_PC_Start();

    /* Turn off the led */
    LedRed_Write(1);
    
    UART_PC_UartPutString("Introduce the values for the RGB colors using this pattern: Wrrrgggbbb.\n\r Example: W255090030\n\r");

    for (;;){
        ch = UART_PC_UartGetChar();    // this command Gets then clears to 0u
        tic = 0;
        
        if (ch == 'W'){             //'W': Write.
			cop = 'Y';
            while (cop != 'X'){
				ch1 = UART_PC_UartGetChar();
				if ((ch1 != 0u)){
                    /* Only digits will be processed. Any other char will be omitted. */
                    if( ch1 >= '0' && ch1 <= '9' ){
    					buffer[tic] = ch1;
    					tic++;
                    }
				}
                /* If the array is completed, then the introduced value us printed on screen and the leds are setted. */
                if (tic==9){
                    UART_PC_UartPutString(buffer);
                    UART_PC_UartPutChar('\r');
                    UART_PC_UartPutChar('\n');
                    /* Prepare the command and obtain the PID and checksum before sending the frame. */
                    getResults(buffer);
                    pid=LIN_PID_Calculator(id);
                    checksum=LIN_Checksum_Calculation(pid, packet);
                    sendFrame();
                    /*********************************************************************************/
                    for(int i=0; i<9; i++)
					    buffer[i] = 0u;
					cop = 'X';
				}
			}
		}
	}
}