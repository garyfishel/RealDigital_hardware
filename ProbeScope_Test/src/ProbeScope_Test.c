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

#if defined (__MULTICORE_MASTER_SLAVE_M0APP) | defined (__MULTICORE_MASTER_SLAVE_M0SUB)
#include "cr_start_m0.h"
#endif

// TODO: insert other include files here
#include "sys_config.h"
#include "app_usbd_cfg.h"
#include "libusbdev.h"
#include <stdio.h>
#include <string.h>
#include "libusbdev.h"
#include <stdlib.h>
#include "board_api.h"



// TODO: insert other definitions and declarations here
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* The size of the packet buffer. */
#define PACKET_BUFFER_SIZE        4096

/* Application defined LUSB interrupt status  */
#define LUSB_DATA_PENDING       _BIT(0)

/* Packet buffer for processing */
static uint8_t g_rxBuff[PACKET_BUFFER_SIZE];


int main(void) {

    SystemCoreClockUpdate();

    //initialize USB ROM
    libusbdev_init(USB_STACK_MEM_BASE, USB_STACK_MEM_SIZE);

    char message[12] = "Hello World!";

    //debug out test
    DEBUGSTR("Hello World");

    strcpy(g_rxBuff, message);

    while (1) {
    		/* wait until host is connected */
    		while (libusbdev_Connected() == 0) {
    			/* Sleep until next IRQ happens */
    			__WFI();
    		}

    		while (libusbdev_Connected()) {

    			if (libusbdev_QueueReadDone() != -1) {

    				/* Dummy process read data ......*/
    				/* requeue read request */
    				libusbdev_QueueReadReq(g_rxBuff, PACKET_BUFFER_SIZE);
    			}

    			if (libusbdev_QueueSendDone() == 0) {
    				/* Queue send request */
    				libusbdev_QueueSendReq(g_rxBuff, PACKET_BUFFER_SIZE);
    			}


    		}
    	}
}
