#include "port_setup.h"

void PORT_Init(PORT_Type * PORTx, uint8 pin, const Port_Config * user)
{
  // Multiplexing
  PORTx->PCR[pin] &= ~PORT_PCR_MUX_MASK;
  PORTx->PCR[pin] |= PORT_PCR_MUX(user->mux);
  
  // Pull Control
  PORTx->PCR[pin] &= ~PORT_PCR_PE_MASK;
  PORTx->PCR[pin] &= ~PORT_PCR_PS_MASK;
  if(user->pull == PULL_UP || user->pull == PULL_DOWN)
  {
    PORTx->PCR[pin] |= PORT_PCR_PE(1);
  }
  if(user->pull == PULL_UP)
  {
    PORTx->PCR[pin] |= PORT_PCR_PS(1);
  }
  
  // IRQC
  PORTx->PCR[pin] &= ~PORT_PCR_IRQC_MASK;
  PORTx->PCR[pin] |= PORT_PCR_IRQC(user->irq);
};

void PORT_EXTI_Config(PORT_Type * PORTx, uint8 pin, Port_IRQType IRQC)
{
  //Setup interrupt
  PORTx->PCR[pin] &= ~PORT_PCR_IRQC_MASK;
  PORTx->PCR[pin] |= PORT_PCR_IRQC(IRQC);       
};

char PORT_Read_Flag(PORT_Type * PORTx, uint8 pin)
{
  return (PORTx->PCR[pin] >> PORT_PCR_ISF_SHIFT) & 0x01;
};

void PORT_EXTI_ClearFlag(PORT_Type * PORTx, uint8 pin)
{
  if( (PORTx->PCR[pin] & PORT_PCR_ISF_MASK ) == PORT_PCR_ISF_MASK)       // flag interrupt =1 
  {
    PORTx->PCR[pin] |= PORT_PCR_ISF(1);                 //clear flag
  }   
};


