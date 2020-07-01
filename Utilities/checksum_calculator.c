/* ========================================
 *
 * LIN v2.2 - Checksum Calculator *
 * ========================================
 * Introduce the PID and the desired 
 * frame to calculate its' checksum.
 *
 * ========================================
*/

#include <stdint.h>
#include <stdio.h>

#define PACKET_SIZE 8u

uint8_t packet[PACKET_SIZE] = {0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t pid = 0x50;

/* Enhanced CheckSum */

uint8_t Checksum_Calculation (uint8_t u8Header, uint8_t *packet){
	uint16_t u16Checksum_Cal = u8Header;
	uint8_t u8Cnt = 0u;
	uint8_t u8Checksum = 0;

	for(u8Cnt = 0u; u8Cnt < PACKET_SIZE; u8Cnt++){
		/* Sum of all bytes */
		u16Checksum_Cal += *(packet + u8Cnt);
	}

	/* Add carry and XOR with FF to invert the bits */
	u8Checksum = (0xFF ^ ((u16Checksum_Cal & 0x00FF) + (u16Checksum_Cal >> 8)));
	return(u8Checksum);
}

int main(void){
	volatile uint8_t checksum;
	checksum = Checksum_Calculation(pid, packet);
	printf("Checksum: %X \n",checksum);
}

/* [] END OF FILE */