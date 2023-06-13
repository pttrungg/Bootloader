#ifndef __UART_H__
#define __UART_H__

#include "MKL46Z4.h"

void UART0_Init(); 

void UART_SendChar(uint8_t data);

void UART_SendString(uint8_t * String);

uint8_t UART_ReceiveChar();

extern void Port_Uart_Init();

void ReceiverInterruptEnable();

void ReceiverInterruptDisable();

#endif

