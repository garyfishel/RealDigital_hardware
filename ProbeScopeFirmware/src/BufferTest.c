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

static int i;

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

static void timer_setup(void)
{
	/* Enable timer 1 clock and reset it */
	Chip_TIMER_Init(LPC_TIMER1);
	Chip_RGU_TriggerReset(RGU_TIMER1_RST);
	while (Chip_RGU_InReset(RGU_TIMER1_RST)) {}

	/* Timer setup for match and interrupt at SAMPLERATE */
	Chip_TIMER_Reset(LPC_TIMER1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
	Chip_TIMER_SetMatch(LPC_TIMER1, 1, (Chip_Clock_GetRate(CLK_MX_TIMER1) / SAMPLERATE));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
	Chip_TIMER_Enable(LPC_TIMER1);

	/* Enable timer interrupt */
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
}

void setupADC() {
	/* Setting up the HSADC clock is more complex than other peripherals.
	   The HSADC clock is driven directly from the CGU/CCU and has limited
	   source and divider options. Because the HSADC clocking is entirely
	   handled outside the HSADC peripheral, example code for setting up
	   the CGU/CCU to get a rough HSADC clock rate is included in this
	   example. */
	setupClock(HSADC_CLOCK_RATE);

	/* Initialize HSADC */
	Chip_HSADC_Init(LPC_ADCHS);

	/* Setup FIFO trip points for interrupt/DMA to 8 samples, no packing */
	Chip_HSADC_SetupFIFO(LPC_ADCHS, 15, true);

	/* Software trigger only, 0x90 recovery clocks, add channel IF to FIFO entry */
	Chip_HSADC_ConfigureTrigger(LPC_ADCHS, HSADC_CONFIG_TRIGGER_SW,
								HSADC_CONFIG_TRIGGER_RISEEXT, HSADC_CONFIG_TRIGGER_NOEXTSYNC,
								HSADC_CHANNEL_ID_EN_ADD, 0x90);

	/* Select both positive and negative DC biasing for input 3 */
	//Chip_HSADC_SetACDCBias(LPC_ADCHS, 3, HSADC_CHANNEL_DCBIAS, HSADC_CHANNEL_DCBIAS);
	Chip_HSADC_SetACDCBias(LPC_ADCHS, 0, HSADC_CHANNEL_DCBIAS, HSADC_CHANNEL_NODCBIAS);

	/* Set low A threshold to 10% and high A threshold to 90% */
	Chip_HSADC_SetThrLowValue(LPC_ADCHS, 0, ((HSADC_MAX_SAMPLEVAL * 1) / 10));
	Chip_HSADC_SetThrHighValue(LPC_ADCHS, 0, ((HSADC_MAX_SAMPLEVAL * 9) / 10));

	/* Set low B threshold to 40% and high B threshold to 60% */
	Chip_HSADC_SetThrLowValue(LPC_ADCHS, 1, ((HSADC_MAX_SAMPLEVAL * 4) / 10));
	Chip_HSADC_SetThrHighValue(LPC_ADCHS, 1, ((HSADC_MAX_SAMPLEVAL * 6) / 10));

	/* Setup data format for 2's complement and update clock settings. This function
	   should be called whenever a clock change is made to the HSADC */
	Chip_HSADC_SetPowerSpeed(LPC_ADCHS, false);

	/* Enable HSADC power */
	Chip_HSADC_EnablePower(LPC_ADCHS);

	/* Setup HSADC table 0 descriptors */
	/* Descriptor entries are mapped as follows */
	/* 0 : mapped to input 0, branch to next descriptor after sample, match time
	   is 0x90 clocks for the initial sample (must be greater than or equal to
		 recovery clocks for auto power-up), test against threshold A */
	Chip_HSADC_SetupDescEntry(LPC_ADCHS, 0, 0, (HSADC_DESC_CH(0) |
												HSADC_DESC_HALT | HSADC_DESC_MATCH(0x95) |
												HSADC_DESC_RESET_TIMER));



	/* Setup HSADC interrupts on group 0 - FIFO trip (full), FIFO overrun
	   error, and descriptor statuses */
	Chip_HSADC_EnableInts(LPC_ADCHS, 0, ( HSADC_INT0_DSCR_DONE |
										  HSADC_INT0_DSCR_ERROR));



	/* Enable HSADC interrupts in NVIC */
	//NVIC_EnableIRQ(ADCHS_IRQn);

	/* Update descriptor tables - needed after updating any descriptors */
	Chip_HSADC_UpdateDescTable(LPC_ADCHS, 0);
	Chip_HSADC_UpdateDescTable(LPC_ADCHS, 1);

}

////////////////////////////////////////////
//DMA Functions
////////////////////////////////////////////

void setupGPDMA() {
	GPDMA_init();
}


////////////////////////////////////////////
//USBD ROM Functions
////////////////////////////////////////////

/*
static void setupUSB() {
	libusbdev_init(USB_STACK_MEM_BASE, USB_STACK_MEM_SIZE);

	// wait until host is connected
	while (libusbdev_Connected() == 0) {
		// Sleep until next IRQ happens
		__WFI();
	}


}
*/


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
	//setupUSB();

	setupADC();

	//samples = (struct CircularBuffer*) malloc(sizeof(struct CircularBuffer));

	sample_pointer = (uint32_t*) malloc(32000*sizeof(uint32_t));

	setupGPDMA();
	//cBufferInit(samples, 32000);

}

// Sample Data
static void getSample() {

	/*
	if (i < 32000) {
		//addToBuffer(samples, lastSample[0]);

		i++;
	}
	*/
	GPDMA_ADC2Mtransfer(sample_pointer, 32000);
}

/*
static void sendSample() {
	for (i = 0; i < 32000; i++) {
		while (libusbdev_QueueSendDone() != 0) {
			//wait for last send to finish
			__WFI();
		}
		//send data point
		libusbdev_QueueSendReq((uint8_t *) &(samples->buffer[(samples->head + i) % 32000]), 32);
	}
}*/


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

    samplingADC();
    getSample();

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
