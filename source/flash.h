#ifndef __FLASH_H__
#define __FLASH_H__

#include "MKL46Z4.h"

void Flash_Write(uint32_t Address, uint32_t Data);

void Flash_EraseSector(uint32_t Address);

uint32_t Flash_Read(uint32_t Address);

#endif

