/*
 * Copyright 2022 NXP
 * NXP confidential.
 * This software is owned or controlled by NXP and may only be used strictly
 * in accordance with the applicable license terms.  By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to
 * be bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>


uint32_t* sample_pointer;

//////////////////////////////////////////////
//Circular buffer Header and global variable
//////////////////////////////////////////////

#include "SampleBuffer.h"

struct CircularBuffer* samples;

//////////////////////////////////////////////
//USBD Rom Headers and variables
//////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include "libusbdev.h"


//////////////////////////////////////////////
//HSADC Variables
//////////////////////////////////////////////

/* HSADC clock rate used for sampling */
#define HSADC_CLOCK_RATE (80 * 1000000)



//////////////////////////////////////////////
//GPDMA Definitions and Variables
//////////////////////////////////////////////

#include "GPDMA.h"

#define FREQTIMER0M0 1000
#define PACKET_BUFFER_SIZE 1024//4096
#define LUSB_DATA_PENDING _BIT(0)

#define DMA_CH 7

static uint8_t g_rxBuff[PACKET_BUFFER_SIZE];
static uint8_t g_txBuff[8192]; //4096
static uint8_t g_txBuff1[8192];
static uint8_t g_txBuff2[8192];
static uint8_t g_txBuff3[8192];
//volatile uint32_t samplevector[32000];
volatile uint32_t numsamples = 0;
volatile bool unhandledReq = false;
volatile bool usbDumpConnected = false;
volatile uint8_t pendingChunks = 0;
volatile uint8_t count = 0;



///////////////////////////////////////////////
//ADC Functions
///////////////////////////////////////////////

/* Base clock lookup table used for finding best HSADC clock rate. Only the
   base clock sources in this table are used as a possible source for the
   HSASC peripheral clock before being divided. Add or remove sources as
   needed, but an exact frequency may not be available unless it's set up
   on an unused PLL or clock input. */
static const CHIP_CGU_CLKIN_T adcBaseClkSources[] = {
	CLKIN_IRC,			/* Usually 12MHz */
	CLKIN_CLKIN,		/* External clock in rate */
	CLKIN_CRYSTAL,		/* Usually 12MHz */
	CLKIN_AUDIOPLL,		/* Unknown, will be 0 if not configured */
	CLKIN_MAINPLL		/* Usually 204MHz, may be too fast to use with a divider */
};

/* Periodic sample rate in Hz */
#define SAMPLERATE (5)

uint32_t freqHSADC = 0;
uint32_t stored_last_0 = 0;

/* Last saved ADC sample for each input */
volatile uint32_t lastSample[6];

static uint32_t getClockRate(int clkIndex, uint32_t maxCGU)
{
	uint32_t clkRate;

	clkRate = Chip_Clock_GetClockInputHz(adcBaseClkSources[clkIndex]);
	clkRate = clkRate / maxCGU;

	return clkRate;
}

/* Clock setup function for generating approximate HSADC clock. Trim this
   example function as needed to get the size down in a production system. */
static void setupClock(uint32_t rate)
{
	CHIP_CGU_IDIV_T freeDivider, bestDivider;
	CHIP_CGU_CLKIN_T divider, mappedCGUDuv, savedClkIn;
	uint32_t bestRate, testRate, maxCGU, savedMaxCGU;
	uint32_t maxCGUDiv;
	int clkIndex;

	/* The HSADC clock (sample) rate is derived from the HSADC base clock
	   divided by the HSADC clock divider (only 1 or 2). The HSADC base
	   clock can be selected from a number of internal clock sources such
	   as the main PLL1, audio PLL, external crystal rate, IRC, RTC, or a
	   CGU divider. Unless a PLL is setup for the exact rate desired, a
	   rate close to the target rate may be the closest approximation. */

	/* Determine if there are any free dividers in the CGU. Assumes an
	   unused divider is attached to CLOCKINPUT_PD. Divider A can only
	   divide 1-4, B/C/D can divide 1-16, E can divider 1-256. */
	freeDivider = CLK_IDIV_A;	/* Dividers only */
	divider = CLKIN_IDIVA;	/* CGU clock input sources */
	bestDivider = CLK_IDIV_LAST;
	while (freeDivider < CLK_IDIV_LAST) {
		CHIP_CGU_CLKIN_T clkIn;

		/* A CGUI divider that is pulled on input down is free */
		clkIn = Chip_Clock_GetDividerSource(freeDivider);
		if (clkIn == CLKINPUT_PD) {
			/* Save available divider and mapped CGU clock divider source */
			bestDivider = freeDivider;
			mappedCGUDuv = divider;
		}

		/* Try next divider */
		freeDivider++;
		divider++;
	}

	/* Determine maximum divider value per CGU divider */
	if (bestDivider != CLK_IDIV_LAST) {
		if (bestDivider == CLK_IDIV_A) {
			maxCGUDiv = 4;
		}
		else if ((bestDivider >= CLK_IDIV_B) && (bestDivider <= CLK_IDIV_D)) {
			maxCGUDiv = 16;
		}
		else {
			maxCGUDiv = 256;
		}
	}
	else {
		/* No CGU divider available */
		maxCGUDiv = 1;
	}

	/* Using the best available maximum CGU and CCU dividers, attempt to
	   find a base clock rate that will get as close as possible to the
	   target rate without going over the rate. */
	savedMaxCGU = 1;
	bestRate = 0xFFFFFFFF;
	for (clkIndex = 0; clkIndex < (sizeof(adcBaseClkSources) / sizeof(CHIP_CGU_CLKIN_T)); clkIndex++) {
		for (maxCGU = 1; maxCGU <= maxCGUDiv; maxCGU++) {
			testRate = getClockRate(clkIndex, maxCGU);
			if (rate >= testRate) {
				if ((rate - testRate) < (rate - bestRate)) {
					bestRate = testRate;
					savedClkIn = adcBaseClkSources[clkIndex];
					savedMaxCGU = maxCGU;
				}
			}
		}
	}

	/* Now to setup clocks */
	if (maxCGUDiv == 1) {
		/* CCU divider and base clock only */
		/* Select best clock as HSADC base clock */
		Chip_Clock_SetBaseClock(CLK_BASE_ADCHS, savedClkIn, true, false);
	}
	else {
		/* CCU divider with base clock routed via a CGU divider */
		Chip_Clock_SetDivider(bestDivider, savedClkIn, savedMaxCGU);
		Chip_Clock_SetBaseClock(CLK_BASE_ADCHS, mappedCGUDuv, true, false);
	}

	/* Enable ADC clock */
	Chip_Clock_EnableOpts(CLK_ADCHS, true, true, 1);
}

//static void timer_setup(void)
//{
//	/* Enable timer 1 clock and reset it */
//	Chip_TIMER_Init(LPC_TIMER1);
//	Chip_RGU_TriggerReset(RGU_TIMER1_RST);
//	while (Chip_RGU_InReset(RGU_TIMER1_RST)) {}
//
//	/* Timer setup for match and interrupt at SAMPLERATE */
//	Chip_TIMER_Reset(LPC_TIMER1);
//	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
//	Chip_TIMER_SetMatch(LPC_TIMER1, 1, (Chip_Clock_GetRate(CLK_MX_TIMER1) / SAMPLERATE));
//	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
//	Chip_TIMER_Enable(LPC_TIMER1);
//
//	/* Enable timer interrupt */
//	NVIC_EnableIRQ(TIMER1_IRQn);
//	NVIC_ClearPendingIRQ(TIMER1_IRQn);
//}

void setupADC() {

	setupClock(HSADC_CLOCK_RATE);

	LPC_ADCHS->INTS[0].CLR_EN   = 0x7F; // disable interrupt 0

	LPC_ADCHS->INTS[0].CLR_STAT = 0x7F; // clear interrupt status

	while(LPC_ADCHS->INTS[0].STATUS & 0x7D); // wait for status to clear, have to exclude FIFO_EMPTY

	LPC_ADCHS->INTS[1].CLR_EN   = 0x7F;

	LPC_ADCHS->INTS[1].CLR_STAT = 0x7F;

	while(LPC_ADCHS->INTS[1].STATUS & 0x7D);

	LPC_ADCHS->POWER_DOWN = 0;
	LPC_ADCHS->FLUSH = 1;
    Chip_HSADC_Init(LPC_ADCHS);

    LPC_ADCHS->FIFO_CFG      = (15 << 1) /* FIFO_LEVEL*/
    						 | (1) /* PACKED_READ*/;

    LPC_ADCHS->DSCR_STS = 1;

    LPC_ADCHS->DESCRIPTOR[0][0] = (1 << 31) /* UPDATE TABLE*/
    							| (1 << 24) /* RESET_TIMER*/
								| (0 << 22) /* THRESH*/
								| (0xA00 << 8) /* MATCH*/
								| (0x10 << 6) /* BRANCH*/;

    LPC_ADCHS->DESCRIPTOR[1][0] = (1 << 31) /* UPDATE TABLE*/
    							| (1 << 24) /* RESET_TIMER*/
								| (0 << 22) /* THRESH*/
								| (0x01 << 8) /* MATCH*/
								| (0x01 << 6) /* BRANCH*/;

    LPC_ADCHS->CONFIG        = (0x90 << 6) /* RECOVERY_TIME*/
                             | (0 << 5) /* CHANNEL_ID_EN*/
                             | (0x01) /* TRIGGER_MASK*/;

    uint8_t DGEC = 0xE;
    LPC_ADCHS->ADC_SPEED     = (DGEC << 16)
    						 | (DGEC << 12)
							 | (DGEC << 8)
							 | (DGEC << 4)
							 | (DGEC);

    //Didn't set threshold registers as they aren't used
    LPC_ADCHS->POWER_CONTROL = (1 << 18) /* BGAP*/
                             | (1 << 17) /* POWER*/
                             | (1 << 10) /* DC in ADC0*/
                             | (1 << 4) | (0x4) /* CRS*/;

}

////////////////////////////////////////////
//DMA Functions
////////////////////////////////////////////

void setupGPDMA() {
	//GPDMA_init();
	//GPDMA_InitADV(sample_pointer);

	//Initialize GPDMA
	Chip_GPDMA_Init(LPC_GPDMA);
	LPC_GPDMA->CONFIG = 0x01;

	while (!(LPC_GPDMA->CONFIG & 0x01));

	//clear dma interrupt and error flag
	LPC_GPDMA->INTTCCLEAR = 0xFF;
	LPC_GPDMA->INTERRCLR = 0xFF;


	//set source and destination addresses
	 LPC_GPDMA->CH[DMA_CH].SRCADDR  =  (uint32_t) &LPC_ADCHS->FIFO_OUTPUT[0];
	 LPC_GPDMA->CH[DMA_CH].DESTADDR = (uint32_t) sample_pointer;

	 LPC_GPDMA->CH[DMA_CH].CONTROL = 4095
		| (0x2                << 12)  // src burst size
		| (0x2                << 15)  // dst burst size
		| (0x2                << 18)  // src transfer width
		| (0x2                << 21)  // dst transfer width
		| (0x1                << 24)  // src AHB master select
		| (0x0                << 25)  // dst AHB master select
		| (0x0                << 26)  // src increment: 0, src address not increment after each transfer
		| (0x1                << 27)  // dst increment: 1, dst address increment after each transfer
		| (0x1                << 31); // terminal count interrupt enable bit: 1, enabled

	 LPC_GPDMA->CH[DMA_CH].CONFIG   =  (0x1                << 0)   // enable bit: 1 enable, 0 disable
		| (0x8     			  << 1)   // src peripheral: set to 8   - HSADC
		| (0x0                << 6)   // dst peripheral: no setting - memory
		| (0x2                << 11)  // flow control: peripheral to memory - DMA control
		| (0x1                << 14)  // IE  - interrupt error mask
		| (0x1                << 15)  // ITC - terminal count interrupt mask
        | (0x0                << 16)  // lock: when set, this bit enables locked transfer
		| (0x1                << 18); // Halt: 1, enable DMA requests; 0, ignore further src DMA req

	 LPC_GPDMA->CH[DMA_CH].LLI      =  0;


}


void startSampling() {
	Chip_HSADC_SWTrigger(LPC_ADCHS);

    // start DMA
    LPC_GPDMA->CH[DMA_CH].CONFIG = (0x1 << 0); // enable bit, 1 enable, 0 disable

    for (int j = 0; j < 2; j++) {
		//wait for transfer to complete
		while (LPC_GPDMA->INTTCSTAT == 1);
    }

    Chip_HSADC_FlushFIFO(LPC_ADCHS);
    uint32_t sts = Chip_HSADC_GetFIFOLevel(LPC_ADCHS);
    Chip_HSADC_DeInit(LPC_ADCHS);
    Chip_GPDMA_DeInit(LPC_GPDMA);
}

////////////////////////////////////////////
//USBD ROM Functions
////////////////////////////////////////////


static void setupUSB() {
	libusbdev_init(USB_STACK_MEM_BASE, USB_STACK_MEM_SIZE);

	// wait until host is connected
	while (libusbdev_Connected() == 0) {
		// Sleep until next IRQ happens
		__WFI();
	}


}



////////////////////////////////////////////
//Basic Device Functions
////////////////////////////////////////////

//initialize device
static void setupHardware() {
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	//initialize USB
	setupUSB();

	setupADC();


	sample_pointer = (uint32_t*) malloc(32000*sizeof(uint32_t));

	setupGPDMA();

	startSampling();

}

// Sample Data
static void getSample() {
	GPDMA_ADC2Mtransfer(sample_pointer, 1);
}


static void sendSample() {
	for (int i = 0; i < 32000; i++) {
		while (libusbdev_QueueSendDone() != 0) {
			//wait for last send to finish
			__WFI();
		}
		//send data point
		libusbdev_QueueSendReq((uint8_t *) &sample_pointer[i], 32);
	}
}


void samplingADC(void) {
	LPC_ADCHS->DSCR_STS = ((0 << 1) | 0); //Descriptor table 0
	LPC_ADCHS->TRIGGER = 1; //SW trigger ADC
}


/////////////Interrupt service Handler Functions//////////////////////////
void TIMER0_IRQHandler(void) //Not Used
{
	if (Chip_TIMER_MatchPending(LPC_TIMER0, 0)) {
		LPC_TIMER0->IR = TIMER_IR_CLR(0);
		LPC_ADCHS->TRIGGER = 1;
	}
}

void ADCHS_IRQHandler(void) //Not Used
{
	uint32_t sts;
	// Get ADC interrupt status on group 0 (TEST)
	sts = Chip_HSADC_GetIntStatus(LPC_ADCHS, 0)
			& Chip_HSADC_GetEnabledInts(LPC_ADCHS, 0);
	// Clear group 0 interrupt statuses
	Chip_HSADC_ClearIntStatus(LPC_ADCHS, 0, sts);
}

void DMA_IRQHandler(void) {
	uint32_t actualLLI;
	static bool on1, on2;

	if (usbDumpConnected == true) { //If USB is in dump mode
		actualLLI = LPC_GPDMA->CH[0].LLI; //Look at LLI in order to know what is the previous full USB buffer and send to PC
		if (actualLLI == (uint32_t) &arrayLLI[0]) {
			while (libusbdev_QueueSendDone() != 0);
			while (libusbdev_QueueSendReq(g_txBuff2, 8192) != 0);
		}
		if (actualLLI == (uint32_t) &arrayLLI[1]) {
			while (libusbdev_QueueSendDone() != 0);
			while (libusbdev_QueueSendReq(g_txBuff3, 8192) != 0);
		}
		if (actualLLI == (uint32_t) &arrayLLI[2]) {
			while (libusbdev_QueueSendDone() != 0);
			while (libusbdev_QueueSendReq(g_txBuff, 8192) != 0);
		}
		if (actualLLI == (uint32_t) &arrayLLI[3]) {
			while (libusbdev_QueueSendDone() != 0);
			while (libusbdev_QueueSendReq(g_txBuff1, 8192) != 0);
		}
	}
	LPC_GPDMA->INTTCCLEAR = LPC_GPDMA->INTTCSTAT;
	//GPDMA_capture(&g_txBuff[0], 128); //Restart DMA operation
	//Board_LED_Set(0,true);
}




int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
#if defined (__MULTICORE_MASTER) || defined (__MULTICORE_NONE)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
#endif
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here

    setupHardware();

    LPC_GPDMA->SYNC = 1;

    while(1) {
    	if (libusbdev_Connected()) {
    		if (libusbdev_QueueReadDone() != -1) {//Enter here when receiving data.
    			//run once mode
    			if(g_rxBuff[0] == 'r') {
    				samplingADC();
    				getSample();
    				sendSample();
    			}
    			//insert other modes here

    		}
    	}
    }


    //sendSample();
    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
