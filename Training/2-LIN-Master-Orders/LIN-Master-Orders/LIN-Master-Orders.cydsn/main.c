#include "project.h"
#include "LIN_Frame_Calculations.h"
#include "ledControl.h"

volatile unsigned char Sensor_data[8];
const uint8_t sync = 0x55;
uint8_t id = 0x10;
uint8_t packet[PACKET_SIZE] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t pid = 0x00;
uint8_t checksum = 0x00;


int main(void){
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    _Bool frame=0;
    UART_MASTER_Start();
    
    for(;;){
        if(SW1_Read()==0){
            id=0x11;
            frame=0;
        }else{
            id=0x10;
            frame=1;
            if((SW2_Read()==0) & (SW3_Read()==0)){
            	packet[0]=0x11;
                RGB_LED_ON_RED;
            }else if((SW2_Read()==0) & (SW3_Read()==1)){
            	packet[0]=0x22;
                RGB_LED_ON_GREEN;
            }else if((SW2_Read()==1) & (SW3_Read()==0)){
            	packet[0]=0x33;
                RGB_LED_ON_BLUE;
            }else{
            	packet[0]=0x00;
                RGB_LED_OFF;
            }
        }
        pid=LIN_PID_Calculator(id);
        checksum=LIN_Checksum_Calculation(pid, packet);
        
        CyDelay(1000);
        
        UART_MASTER_UartSendBreakBlocking(13);                              // Break
        UART_MASTER_UartPutChar(sync);                                      // Sync (0x55)
        UART_MASTER_UartPutChar(pid);                                       // Protected ID (PID). Se obtiene a partir del ID.
        if(frame==1){
            UART_MASTER_UartPutChar(packet[0]);                             // Command
            UART_MASTER_UartPutChar(packet[1]);                             // Fill. Relleno para completar los 8 bits de la trama.
            UART_MASTER_UartPutChar(packet[2]);
            UART_MASTER_UartPutChar(packet[3]);
            UART_MASTER_UartPutChar(packet[4]);
            UART_MASTER_UartPutChar(packet[5]);
            UART_MASTER_UartPutChar(packet[6]);
            UART_MASTER_UartPutChar(packet[7]);
            UART_MASTER_UartPutChar(checksum);                              // Checksum. En LIN 2.2 se obtiene a partir del PID y del CMD enviado.*/
        }

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