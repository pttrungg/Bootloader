#include "clock.h"

void Clock_Init(SIM_SCGC5_t Portx)
{
  SIM->SCGC5 |= ( 1 << (Portx + 9u) );
};

void Systick_Init(uint32_t time)                			// ms
{
  // Configure Clock & Clock Source
  MCG->C1 &= ~MCG_C1_CLKS_MASK;
  MCG->C1 |= MCG_C1_CLKS(2u);                             	//external clock for core 
  
  SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV1_MASK;
  SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(7);              		// Core Clock - 1MHz  
  
  SysTick->CTRL |= (1 << 2u);                    			//core clock
  SysTick->LOAD = time*1000u;
  
  SysTick->VAL = 0;             							//reset current value   
};
