#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "MKL46Z4.h"

typedef enum{
  Clock_PORTA   = 0u,
  Clock_PORTB   = 1u,
  Clock_PORTC   = 2u,
  Clock_PORTD   = 3u,
  Clock_PORTE   = 4u,
}SIM_SCGC5_t;

void Clock_Init(SIM_SCGC5_t Portx);

void Systick_Init(uint32_t time);

#endif

