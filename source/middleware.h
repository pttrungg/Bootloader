
#ifndef __MIDDLEWARE_H__
#define __MIDDLEWARE_H__

#include "MKL46Z4.h"
#include "port_setup.h"
#include "gpio_setup.h"
#include "clock.h"

void LED_RED_Init();

void LED_GREEN_Init();

void BTN1_Init();

void BTN2_Init();

void Toggle_LED_RED();

void Toggle_LED_GREEN();

void BTN_Interrupt_Config();

void Port_Uart_Init();          //openSDA

#endif


