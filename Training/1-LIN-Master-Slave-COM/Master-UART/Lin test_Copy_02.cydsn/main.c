/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
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
        UART_MASTER_UartPutChar(0x50);                                      // Protected ID (PID). Se obtiene a partir del ID.
        UART_MASTER_UartPutChar(0x22);                                      // Command
        //UART_MASTER_UartPutChar(0x33);                                    // Command
        UART_MASTER_UartPutChar(0xFF);                                      // Fill. Relleno para completar los 8 bits de la trama.
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        UART_MASTER_UartPutChar(0xFF);
        //UART_MASTER_UartPutChar(0x7C);
        UART_MASTER_UartPutChar(0x8D);                                      // Checksum. En LIN 2.2 se obtiene a partir del PID y del CMD enviado.
        

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