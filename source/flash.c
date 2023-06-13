#include "flash.h"
#include "srec.h"       //queue

void Flash_Write(uint32_t Address, uint32_t Data)
{
  if(Address >= 0x400 && Address <= 0x40F)
  {
    return;
  }
  
  while( (FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
  
  if( (FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK ) == FTFA_FSTAT_ACCERR_MASK )
  {
    FTFA->FSTAT |= FTFA_FSTAT_ACCERR(1);
  }
  if( (FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) == FTFA_FSTAT_FPVIOL_MASK)
  {
    FTFA->FSTAT |= FTFA_FSTAT_FPVIOL(1);
  }
  
  FTFA->FCCOB0 = 0x06;         //write
  
  FTFA->FCCOB1 = (uint8_t)(Address >> 16u);
  FTFA->FCCOB2 = (uint8_t)(Address >> 8u);
  FTFA->FCCOB3 = (uint8_t)Address;

  FTFA->FCCOB4 = (uint8_t)(Data >> 24u);
  FTFA->FCCOB5 = (uint8_t)(Data >> 16u); 
  FTFA->FCCOB6 = (uint8_t)(Data >> 8u);
  FTFA->FCCOB7 = (uint8_t)Data;
  
  __disable_irq();
  FTFA->FSTAT |= FTFA_FSTAT_CCIF(1);            //clear to start write
  
  while( (FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
  __enable_irq();
}

void Flash_EraseSector(uint32_t Address)
{
  if( (Address >= 0x400u) && (Address <= 0x7FFu))       //sector 1
  {
    return;
  }     
     
  while( (FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
  
  if( (FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK ) == FTFA_FSTAT_ACCERR_MASK )
  {
    FTFA->FSTAT |= FTFA_FSTAT_ACCERR(1);
  }
  if( (FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) == FTFA_FSTAT_FPVIOL_MASK)
  {
    FTFA->FSTAT |= FTFA_FSTAT_FPVIOL(1);
  }
  
  FTFA->FCCOB0 = 0x09;          //erase a sector
  
  FTFA->FCCOB1 = (uint8_t)(Address >> 16u);
  FTFA->FCCOB2 = (uint8_t)(Address >> 8u);
  FTFA->FCCOB3 = (uint8_t)Address;
  
  __disable_irq();
  FTFA->FSTAT |= FTFA_FSTAT_CCIF(1);
  
  while( (FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
  __enable_irq();
}

uint32_t Flash_Read(uint32_t Address)
{
  return *(uint32_t *) Address;
}

