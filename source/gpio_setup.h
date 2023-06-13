
#ifndef __GPIO_H__
#define __GPIO_H__

#include "MKL46Z4.h"

typedef enum{
  GPIO_PDDR_INPUT        =0,
  GPIO_PDDR_OUTPUT       =1,
}GPIO_PDDR_Type;

typedef enum{
  GPIO_PDOR_LOGIC_0       =0,
  GPIO_PDOR_LOGIC_1       =1,
}GPIO_PDOR_Type;

typedef struct{
  GPIO_PDDR_Type   pdd;
  GPIO_PDOR_Type   pdo;
}GPIO_Config;

void GPIO_Init(GPIO_Type * GPIOx, uint8_t pin, const GPIO_Config * user);

void GPIO_WritePin(GPIO_Type * GPIOx, uint8_t pin);     // write =1 

void GPIO_ClearPin(GPIO_Type * GPIOx, uint8_t pin);     // clear =0

void GPIO_TogglePin(GPIO_Type * GPIOx, uint8_t pin);    

char GPIO_ReadPin(GPIO_Type * GPIOx, uint8_t pin);

#endif


