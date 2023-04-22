#include "Signal_Generator.h"

int main(void) {

	//initialize variables

	//pointer to fpga output
	uint32_t *samplep = (uint32_t *) Sample_Data;
	uint32_t *triggerp = (uint32_t *) BTN_Data;
	//32k sample array
	uint32_t *sample_arr = (uint32_t *) malloc(500*sizeof(uint32_t));

	//32k time of sample array
	double *time_arr = (double *) malloc (500*sizeof(double));

	//get pointer to processor global time
	XTime* processor_time = (XTime *) malloc (sizeof(XTime));
	XTime_GetTime(processor_time);

	//configure UART
	configure_uart1();

	//continuous loop
	for (;;) {
		//check for trigger (btn0 is pressed)
		if((*triggerp & 1) == 1) {
			//initialize start time
			XTime_SetTime(0x0000000000000000);

			//store 32k samples and times to memory
			for (int i = 0; i < 500; i++) {
				sample_arr[i] = *samplep;
				time_arr[i] = get_sample_time(processor_time);
			}

			//send sample data to host computer over uart
			save_sample(time_arr, sample_arr);

		}
	}
}

double get_sample_time(XTime* processor_time) {
	XTime_GetTime(processor_time);
	return (double) *processor_time;
}

void save_sample(double* time_addr, uint32_t* sample_addr) {
	//send file header
	uart1_sendstr("Time,");
	uart1_sendchar(' ');
	uart1_sendstr("Voltage\n");
	char str[32] = "";
	//send data line by line
	for (int i = 0; i < 500; i++) {
		sprintf( str, "%f,", time_addr[i]);
		uart1_sendstr(str);
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
	for (int i = 0; i <= 32; i++) {
		uart1_sendchar(buffer[i]);
		if (buffer[i] == ',' || buffer[i] == '\n') {
			return;
		}
	}

}
