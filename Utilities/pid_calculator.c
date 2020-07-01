/* ======================================
 *
 * LIN v2.2 - Protected ID Calculator *
 * ======================================
 * Introduce the ID to calculate the PID.
 *
 * ======================================
*/

#include <stdint.h>
#include <stdio.h>

uint8_t LIN_ID_Parity_Calculator(uint8_t data){
	uint8_t parity_bit_even = 0x00;
	uint8_t parity_bit_odd = 0x00;
	uint8_t result = 0x00;

	parity_bit_even = ((data ^ (data>>1))^((data>>2)^(data>>4)));
	parity_bit_odd = (~(((data>>1) ^ (data>>3))^((data>>4)^(data>>5))));
	result = (parity_bit_odd<<1)|(parity_bit_even);
	return result;
}

int main(void){
	volatile uint8_t pid_tmp=0x00;
	volatile uint8_t pid=0x00;
	volatile uint8_t id=0x10;
	pid_tmp=(LIN_ID_Parity_Calculator(id));
	pid=(uint8_t)((id)|(pid_tmp<<6));
	printf("PID: %X \n", pid);
}