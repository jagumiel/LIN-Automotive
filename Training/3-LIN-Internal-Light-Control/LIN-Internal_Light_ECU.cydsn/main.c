/* ========================================
 * _______________
 * LIN-Master-UART
 * ---------------
 * This is a first approach to send a LIN
 * frame as Master to the Slave. For this
 * porpouse a UART communication is used.
 * A LIN transceiver will be needed for
 * this project.
 *
 * This program just send a prepared frame
 * every second. It is just a test.
 *
 * Cypress CY8CKIT-042 and CY8CKIT-026 are
 * used in this project.
 *
 * ========================================
*/

#include "project.h"
#include "LIN_Frame_Calculations.h"
#include "ledControl.h"

volatile unsigned char Sensor_data[8];
const uint8_t sync = 0x55;
uint8_t id = 0x10;
uint8_t packet[PACKET_SIZE] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t pid = 0x00;
uint8_t checksum = 0x00;

void sendFrame(void){
    UART_MASTER_UartSendBreakBlocking(13);                          // Break
    UART_MASTER_UartPutChar(sync);                                  // Sync (0x55)
    UART_MASTER_UartPutChar(pid);                                   // Protected ID (PID). Se obtiene a partir del ID.
    UART_MASTER_UartPutChar(packet[0]);                             // Command
    UART_MASTER_UartPutChar(packet[1]);                             // Fill. Relleno para completar los 8 bits de la trama.
    UART_MASTER_UartPutChar(packet[2]);
    UART_MASTER_UartPutChar(packet[3]);
    UART_MASTER_UartPutChar(packet[4]);
    UART_MASTER_UartPutChar(packet[5]);
    UART_MASTER_UartPutChar(packet[6]);
    UART_MASTER_UartPutChar(packet[7]);
    UART_MASTER_UartPutChar(checksum);                              // Checksum. En LIN 2.2 se obtiene a partir del PID y del CMD enviado.*/
    
    Sensor_data[0] = UART_MASTER_UartGetChar();                     // 1. Byte
    Sensor_data[1] = UART_MASTER_UartGetChar();                     // 2. Byte
    Sensor_data[2] = UART_MASTER_UartGetChar();                     // 3. Byte
    Sensor_data[3] = UART_MASTER_UartGetChar();                     // 4. Byte
    Sensor_data[4] = UART_MASTER_UartGetChar();                     // 5. Byte
    Sensor_data[5] = UART_MASTER_UartGetChar();                     // 6. Byte
    Sensor_data[6] = UART_MASTER_UartGetChar();                     // 7. Byte
    Sensor_data[7] = UART_MASTER_UartGetChar();                     // 8. Byte
    
    LedRed_Write(1);
    CyDelay(500);
    LedRed_Write(0);
    CyDelay(500);
    
}

void leftDoorEvent(){
    if(Button_Left_Read()==1){
        /* Left Door Opened*/
        packet[0]=0x01;
    }else{
        /* Left Door closed*/
        packet[0]=0x00;
    }
}

void rightDoorEvent(){
    if(Button_Right_Read()==1){
        /* Right Door Opened*/
        packet[0]=0x11;
    }else{
        /* Right Door Closed*/
        packet[0]=0x10;
    }
}


CY_ISR(Button_Left_ISR_Handler){
    leftDoorEvent();
    pid=LIN_PID_Calculator(id);
    checksum=LIN_Checksum_Calculation(pid, packet);
    sendFrame();
    Button_Left_ClearInterrupt();
}

CY_ISR(Button_Right_ISR_Handler){
    rightDoorEvent();
    pid=LIN_PID_Calculator(id);
    checksum=LIN_Checksum_Calculation(pid, packet);
    sendFrame();
    Button_Right_ClearInterrupt();
}


int main(void){
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Start UART/LIN Master */
    UART_MASTER_Start();
    
    /* Start ISRs */
    Button_Left_ISR_StartEx(Button_Left_ISR_Handler);
    Button_Right_ISR_StartEx(Button_Right_ISR_Handler);

    /* Turn off the led */
    LedRed_Write(0);
    
    
    for(;;){
        /* No Code */
    }
}