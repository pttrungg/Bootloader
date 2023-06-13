#ifndef __PORT_H__
#define __PORT_H__

#include "MKL46Z4.h"

typedef unsigned int uint32;
typedef unsigned char uint8;

typedef enum{
  DISABLE               = 0,
  DMA_RISING_EDGE       = 1,
  DMA_FALLING_EDGE      = 2,
  DMA_EITHER_EDGE       = 3,
  IRQ_LOGIC_ZERO        = 8,
  IRQ_RISING_EDGE       = 9,
  IRQ_FALLING_EDGE      = 10,
  IRQ_EIHTER_EDGE       = 11,
  IRQ_LOGIC_ONE         = 12
}Port_IRQType;

typedef enum{
  ANALOG = 0,
  GPIO   = 1,
  ALT2   = 2,
  ALT3   = 3,        
  ALT4   = 4,
  ALT5   = 5,
  ALT6   = 6,
  ALT7   = 7
}Port_MuxType;

typedef enum{
  PULL_DISABLE  = 0,
  PULL_UP       = 1,
  PULL_DOWN     = 2
}Port_PullType;

typedef struct{
  Port_MuxType          mux;
  Port_PullType         pull;
  Port_IRQType          irq;
}Port_Config;

void PORT_Init(PORT_Type * PORTx, uint8 pin, const Port_Config * user);

void PORT_EXTI_Config(PORT_Type * PORTx, uint8 pin, Port_IRQType IRQC);

char PORT_Read_Flag(PORT_Type * PORTx, uint8 pin);

void PORT_EXTI_ClearFlag(PORT_Type * PORTx, uint8 pin);

#endif