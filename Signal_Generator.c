#include "Signal_Generator.h"

int main(void) {

	//initialize variables

	//pointer to fpga output
	uint32_t *samplep = (uint32_t *) Sample_Data;
	uint32_t *triggerp = (uint32_t *) BTN_Data;
	//32k sample array
	uint32_t *sample_arr = (uint32_t *) malloc(32000*sizeof(uint32_t));

	//32k time of sample array
	//uint32_t time_arr[32000];

	//continuous loop
	for (;;) {
		//check for trigger (btn0 is pressed)
		if((*triggerp & 1) == 1) {
			//store 32k samples and times to memory
			for (int i = 0; i < 32000; i++) {
				sample_arr[i] = *samplep;
				//time_arr[i] = ;
			}

			//send sample data to host computer over uart
			save_sample(sample_arr);

		}
	}
}


void save_sample(uint32_t* sample_addr) {
	//send file header
	uart1_sendstr("Time,");
	uart1_sendchar(' ');
	uart1_sendstr("Voltage\n");
	char str[32] = "";
	//send data line by line
	for (int i = 0; i < 32000; i++) {
		//uart1_sendstr(time_addr[i]);
		//uart1_sendchar(',');
		sprintf( str, "%" PRIu32 "\n", sample_addr[i]);
		uart1_sendstr(str);
	}
}

void configure_uart1() {
	reset_uart1();

	//set no parity: bits [5:3] = 1xx
	uint32_t *MR = (uint32_t*)UART1_MR;
	*MR = 0b00100000;

	//enable rx and tx
	uint32_t *CR = (uint32_t*)UART1_CR;
	*CR = 0b00010100;

	//Set Baud Gen value
	uint32_t *BG = (uint32_t*)UART1_UBG;
	*BG = 124;

	//set Baud Rate value
	uint32_t *BD = (uint32_t*)UART1_UBD;
	*BD = 6;
}

void reset_uart1() {

	//access the control register of the UART
	uint32_t *CR = UART1_CR;
	//set the value to 0b11
	*CR = 0b11;
}

void uart1_sendchar(char data) {
	//wait for room in fifo
	uint32_t *SR = (uint32_t*)UART1_SR;

	//wait until bit 4 of the status register = 0
	while ((*SR & 0b10000) == 0b10000) {
		//wait
		;;
	}

	//write to tx fifo
	uint32_t *DATA = (uint32_t*)UART1_Data;
	*DATA = data;
}

void uart1_sendstr(char buffer[32]) {
	for (int i = 0; i <= sizeof(*buffer)/sizeof(buffer[0]); i++) {
		uart1_sendchar(buffer[i]);
	}

}
