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

volatile unsigned char Sensor_data[8];

int main(void){
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_MASTER_Start();

    for(;;){

        CyDelay(1000);

        
        UART_MASTER_UartSendBreakBlocking(13);                              // Break
        UART_MASTER_UartPutChar(0x55);                                      // Sync
        UART_MASTER_UartPutChar(0x50);                                      // Protected ID (PID). It is obtained from the Slave ID.
        UART_MASTER_UartPutChar(0x22);                                      // Command
        //UART_MASTER_UartPutChar(0x33);                                    // Command
        UART_MASTER_UartPutChar(0xFF);                                      // Fill. 0xFF is used to complete the frame length.
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        //UART_MASTER_UartPutChar(0x7C);
        UART_MASTER_UartPutChar(0x8D);                                      // Checksum. As specified by LIN v2.2 protocol.
        

        Sensor_data[0] = UART_MASTER_UartGetByte();                         // 1. Byte
        Sensor_data[1] = UART_MASTER_UartGetByte();                         // 2. Byte
        Sensor_data[2] = UART_MASTER_UartGetByte();                         // 3. Byte
        Sensor_data[3] = UART_MASTER_UartGetByte();                         // 4. Byte
        Sensor_data[4] = UART_MASTER_UartGetByte();                         // 5. Byte
        Sensor_data[5] = UART_MASTER_UartGetByte();                         // 6. Byte
        Sensor_data[6] = UART_MASTER_UartGetByte();                         // 7. Byte
        Sensor_data[7] = UART_MASTER_UartGetByte();                         // 8. Byte
    }
}