/*
 * main.c
 *
 *  Created on: Jan 28, 2019
 *      Author: parallels
 */
#include <stdio.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xil_types.h"
#include "xil_assert.h"
#include "xuartps_hw.h"

#define UART_BASEADDR		XPAR_XUARTPS_0_BASEADDR
#define UART_CLOCK_HZ		XPAR_XUARTPS_0_CLOCK_HZ
#define TEST_BUFFER_SIZE	16
#define CHAR_ESC		    0x1b	/* 'ESC' character is used as terminator */

u8 SendBuffer[TEST_BUFFER_SIZE];
u8 ReceiveBuffer[TEST_BUFFER_SIZE];

int main(void){

	int reg_state;
	u8 RXbyte;
	u8 TXbyte;
	u32 CntrlRegister;

	reg_state = 0;

	while(1){
		switch (reg_state){
		case 0: /* init */
			CntrlRegister = XUartPs_ReadReg(UART_BASEADDR,XUARTPS_FIFO_OFFSET);

			/* Enable TX and RX for the device */
			XUartPs_WriteReg(UART_BASEADDR, XUARTPS_CR_OFFSET,((CntrlRegister & ~XUARTPS_CR_EN_DIS_MASK) | XUARTPS_CR_TX_EN | XUARTPS_CR_RX_EN));

			reg_state = 1;
			break;

		case 1: /* listen */
			/* Listen for data to come in */
			while (!XUartPs_IsReceiveData(UART_BASEADDR)){
				reg_state = 1;
			};

			reg_state = 2;
			break;

		case 2: /* receive */
			RXbyte = XUartPs_ReadReg(UART_BASEADDR,XUARTPS_FIFO_OFFSET);
			TXbyte = RXbyte;

			reg_state = 3;
			break;

		case 3: /* send */
			XUartPs_WriteReg(UART_BASEADDR,XUARTPS_FIFO_OFFSET,TXbyte);

			reg_state = 1;
			break;

		default:
			reg_state = 1;
			break;
		}
	}
}
