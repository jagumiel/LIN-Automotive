/* ===========================================
 * ___________________________________________
 *     LIN: Sonceboz Stepper Motor Control
 * -------------------------------------------
 * This is the first project using a real LIN
 * slave. Sonceboz 5877R1007 stepper motor is
 * used for this test. The aim is to calibrate
 * and change the position of the motor.
 *
 * A LIN transceiver will be needed for
 * this project.
 *
 * Cypress CY8CKIT-042 and CY8CKIT-026 are
 * used in this project with Sonceboz motor.
 *
 * ===========================================
*/

#include <stdlib.h> /* Needed for "atoi()" function */
#include "project.h"
#include "LIN_Frame_Calculations.h"

volatile unsigned char sonceboz_ans[10];
const uint8_t sync = 0x55;
uint8_t id = 0x10;

uint8_t slaveResponse[10]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};;
//uint8_t packet1[PACKET_SIZE] = {0x15, 0xAF, 0x80, 0x25, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t pos_open[PACKET_SIZE]  = {0xF0, 0x01, 0x00, 0x00};
uint8_t pos_close[PACKET_SIZE] = {0x00, 0x01, 0x00, 0x00};
uint8_t pos_calib[PACKET_SIZE] = {0x00, 0x00, 0x00, 0x00};
uint8_t sonceboz_pos=0x00;
uint8_t sonceboz_status=0x00;
_Bool sonceboz_vaneCalAct=0b1;

uint8_t pid = 0x00;
uint8_t checksum = 0x00;
uint8_t calibrationEnable[3]= {0x00, 0x00, 0x00};
uint8_t parar=1;


void sendFrame(uint8_t packet[]){
    NSLP_Write(1);
    id=0x32;
    pid=LIN_PID_Calculator(id);
    checksum=LIN_Checksum_Calculation(0x00, packet);
    UART_MASTER_UartSendBreakBlocking(13);                          // Break
    UART_MASTER_UartPutChar(sync);                                  // Sync (0x55)
    UART_MASTER_UartPutChar(pid);                                   // Protected ID (PID). Se obtiene a partir del ID.
    for(int i=0; i<4; i++){
        UART_MASTER_UartPutChar(packet[i]);                         // Command Frame
    }
    UART_MASTER_UartPutChar(checksum);                              // Checksum. En LIN 2.2 se obtiene a partir del PID y del CMD enviado.*/
    /* Delay is needed before setting NSLP to 0. Minimum value is 7ms. If there is no delay the frame will not reach the slave. */
    CyDelay(7);
    NSLP_Write(0);
}

void getSlaveResponse(){
    id=0x34;
    pid=LIN_PID_Calculator(id);
    NSLP_Write(1);
    UART_MASTER_UartSendBreakBlocking(13);                          // Break
    UART_MASTER_UartPutChar(sync);                                  // Sync (0x55)
    UART_MASTER_UartPutChar(pid);                                   // Protected ID (PID). Se obtiene a partir del ID.

    
    for (int i=0; i<9; i++){
        slaveResponse[i] = UART_MASTER_UartGetChar();
    }
    
    //bitStatus = (num >> n) & 1;
    /*sonceboz_pos=slaveResponse[2];
    sonceboz_status=slaveResponse[3];
    sonceboz_vaneCalAct=(sonceboz_status >> 7) & 1;
    CyDelay(7);*/
    NSLP_Write(0);
}

void calibrate(){
    sendFrame(pos_open);
    while(sonceboz_vaneCalAct!=0x00){
        sendFrame(pos_open);
        
        for (int i=0; i<10; i++){  
            getSlaveResponse();
            CyDelay(25);
            sendFrame(pos_open);
            CyDelay(25);
        }
        sonceboz_pos=slaveResponse[3];
        sonceboz_status=slaveResponse[4];
        sonceboz_vaneCalAct=(sonceboz_status >> 7) & 1;
        CyDelay(500);
    }
    for (int i=0; i<10; i++){
        getSlaveResponse();
        CyDelay(25);
        sendFrame(pos_close);
        CyDelay(25);
        CyDelay(500);
    }
    //CyDelay(500);
    for (int i=0; i<10; i++){
        getSlaveResponse();
        CyDelay(25);
        sendFrame(pos_calib);
        CyDelay(25);
    }
}


int main(void){
    
    /* Enable global interrupts. */
    CyGlobalIntEnable;

    /* Start UART/LIN Master */
    UART_MASTER_Start();
    calibrate();
    
    /*while(1){
        getSlaveResponse();
    }*/
    
    for (;;){
        uint8_t trama1[4]={0x32, 0x00, 0x00, 0x00};
        uint8_t trama2[4]={0xc0, 0x00, 0x00, 0x00};
        sendFrame(trama1);
        CyDelay(2500);
        sendFrame(trama2);
        CyDelay(2500);
        //id=0x50;
        //pid=LIN_PID_Calculator(id);
        

	}
}