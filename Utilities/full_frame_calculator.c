#include <stdio.h>
#include <stdint.h>

#define PACKET_SIZE 8u

uint8_t packet[PACKET_SIZE];
uint8_t id=0x00;

int i=0;

/*Calculates the Parity bits from the ID.*/
uint8_t LIN_ID_Parity_Calculator(uint8_t data){
	uint8_t parity_bit_even = 0x00;
	uint8_t parity_bit_odd = 0x00;
	uint8_t result = 0x00;

	parity_bit_even = ((data ^ (data>>1))^((data>>2)^(data>>4)));
	parity_bit_odd = (~(((data>>1) ^ (data>>3))^((data>>4)^(data>>5))));
	result = (parity_bit_odd<<1)|(parity_bit_even);
	return result;
}

/*Calculates the checksum from the PID and the data frame.*/
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

	volatile uint8_t pid_tmp=0x00;
	volatile uint8_t pid=0x00;
	volatile uint8_t id=0x10;
	volatile uint8_t checksum;

	printf("Enter the hexadecimal LIN ID without \"0x\":");
	scanf("%2x", &id);
	printf("Enter the hexadecimal data frame without \"0x\" and without blank spaces nor separators:");
	for(i=0; i<PACKET_SIZE; i++){
		scanf("%2x", &packet[i]);
	}

	//Generates the PID using the ID and the parity bits calculation
	pid_tmp=(LIN_ID_Parity_Calculator(id));
	pid=(uint8_t)((id)|(pid_tmp<<6));

	checksum = Checksum_Calculation(pid, packet);
	printf("PID: %X \n", pid);
	for(i=0; i< PACKET_SIZE; i++){
		printf("Data Frame = 0x%X\n", packet[i]);
	}
	printf("Checksum: %X \n",checksum);
	return 0;
}