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

#include <stdint.h>
#include <stdio.h>

#define PACKET_SIZE 4u


uint8_t LIN_ID_Parity_Calculator(uint8_t data){
	uint8_t parity_bit_even = 0x00;
	uint8_t parity_bit_odd = 0x00;
	uint8_t result = 0x00;

	parity_bit_even = ((data ^ (data>>1))^((data>>2)^(data>>4)));
	parity_bit_odd = (~(((data>>1) ^ (data>>3))^((data>>4)^(data>>5))));
	result = (parity_bit_odd<<1)|(parity_bit_even);
	return result;
}

int LIN_PID_Calculator(uint8_t id){
	volatile uint8_t pid_tmp=0x00;
	volatile uint8_t pid=0x00;
	pid_tmp=(LIN_ID_Parity_Calculator(id));
	pid=(uint8_t)((id)|(pid_tmp<<6));
	return pid;
}

uint8_t LIN_Checksum_Calculation (uint8_t u8Header, uint8_t *packet){
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

/* [] END OF FILE */
