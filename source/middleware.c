#include "middleware.h"

const Port_Config port_button ={
   .mux = GPIO,
   .pull = PULL_UP
   //.irq = IRQ_FALLING_EDGE,
};

/*const GPIO_Config gpio_button ={
  .pdd = GPIO_PDDR_INPUT,
};*/

const Port_Config port_led ={
  .mux = GPIO,
};

const GPIO_Config gpio_led ={
  .pdd = GPIO_PDDR_OUTPUT,
  .pdo = GPIO_PDOR_LOGIC_1,
};

const Port_Config port_uart = {
  .mux = ALT2,
  .pull = PULL_UP
};

void Port_Uart_Init()   //openSDA
{
  Clock_Init(Clock_PORTA);
  PORT_Init(PORTA, 2, &port_uart);
  PORT_Init(PORTA, 1, &port_uart);
};

void LED_RED_Init()
{
  Clock_Init(Clock_PORTE);
  PORT_Init(PORTE, 29, &port_led);
  GPIO_Init(GPIOE, 29, &gpio_led);
};

void LED_GREEN_Init()
{
  Clock_Init(Clock_PORTD);
  PORT_Init(PORTD, 5, &port_led);
  GPIO_Init(GPIOD, 5, &gpio_led);
};

void BTN1_Init()
{
  Clock_Init(Clock_PORTC);
  PORT_Init(PORTC, 3, &port_button);
};

void BTN2_Init()
{
  Clock_Init(Clock_PORTC);
  PORT_Init(PORTC, 12, &port_button);
};

void Toggle_LED_RED()
{
  GPIO_TogglePin(GPIOE, 29);
};

void Toggle_LED_GREEN()
{
  GPIO_TogglePin(GPIOD, 5);
};

void BTN_Interrupt_Config(){
  PORT_EXTI_Config(PORTC,3,IRQ_FALLING_EDGE);
  PORT_EXTI_Config(PORTC,12,IRQ_FALLING_EDGE);
  
  NVIC_EnableIRQ(PORTC_PORTD_IRQn );    //enable interrupt
};


