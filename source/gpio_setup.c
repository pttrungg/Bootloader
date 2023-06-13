#include "gpio_setup.h"

void GPIO_Init(GPIO_Type * GPIOx, uint8_t pin, const GPIO_Config * user)        
{       
  GPIOx->PDDR |= (user->pdd << pin);
  GPIOx->PDOR |= (user->pdo << pin);
};

void GPIO_WritePin(GPIO_Type * GPIOx, uint8_t pin)
{
  GPIOx->PSOR |= (1 << pin);
};

void GPIO_ClearPin(GPIO_Type * GPIOx, uint8_t pin)      //clear data in a pin (=0)
{
  GPIOx->PCOR |= (1 << pin);
};

void GPIO_TogglePin(GPIO_Type * GPIOx, uint8_t pin)     //toogle data in apin
{
  GPIOx->PTOR |= (1 << pin);
};

char GPIO_ReadPin(GPIO_Type * GPIOx, uint8_t pin)
{
  return (((GPIOx->PDIR)>> pin) & 0x01);
};
