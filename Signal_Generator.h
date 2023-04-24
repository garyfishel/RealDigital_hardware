#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <xtime_l.h>


//UART locations
#define UART1_CR 0xE0001000
#define UART1_MR 0xE0001004

#define UART1_UBG 0xE0001018
#define UART1_UBD 0xE0001034

#define UART1_SR 0xE000102C
#define UART1_Data 0xE0001030

//FPGA AXI base address
#define Sample_Data 0x43C00000

//AXI Button Address
#define BTN_Data 0x43C00004

//returns a double value of the current global processor time
float get_sample_time(XTime* processor_time);

//Takes the 32k samples stored in the sample array and saves them into a csv file at fptr
void save_sample(float* time_addr, uint32_t* sample_addr);

//configures UART1 for 115200 baud, 8-data bits, no parity, 1 stop bit.
void configure_uart1();

//resets UART1
void reset_uart1();

//Sends 'data' over uart.
//Blocks until data is placed in transmit FIFO.
void uart1_sendchar(char data);

//sends the null-terminated string in 'buffer' through uart1
void uart1_sendstr(char buffer[]);

