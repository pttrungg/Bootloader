#include "uart.h"

uint8_t FlagStartRecord = 0, FlagType = 0, Flag = 0;

void UART0_Init()
{
  Port_Uart_Init();
  SIM->SCGC4 |= SIM_SCGC4_UART0(1u);     

  MCG->C1 |= MCG_C1_IRCLKEN(1u);                 		//enable MCGIRCLK
  MCG->C2 |= MCG_C2_IRCS(1u);                    		//fast internal = 2mhz
  SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_UART0SRC(3u);  				//Clock for Baudrate - MCGIRCLK

  //frame
  UART0->C1 &= ~UART0_C1_M_MASK;        				//8 bit data
  UART0->C1 &= ~UART0_C1_PE_MASK;       				//parity disable
  UART0->BDH &= ~UART0_BDH_SBNS_MASK;   				//1 bit stop

  // baudrate = 9600.
  // 9600 = 2.000.000 / SBR[12:0] × (OSR+1)
  // OSR = 3 -> SBR = 52
  UART0->BDH &= ~UART0_BDH_SBR_MASK;
  UART0->BDL = 52u;
  
  UART0->C4 = (UART0->C4 & ~UART0_C4_OSR_MASK) | UART0_C4_OSR(3u);      //osr = 4

  UART0->C5 |= UART0_C5_BOTHEDGE(1u);            		//Both edge for receive when OSR = 3 -> 7
  
  UART0->C2 |= UART0_C2_TE(1u);
  UART0->C2 |= UART0_C2_RE(1u);

}

void UART_SendChar(uint8_t data)
{
  UART0->D = data;      								//write data -> resgister data
  
  while( (UART0->S1 & UART0_S1_TDRE_MASK ) == 0);  		//wait bufer empty

}

void UART_SendString(uint8_t * String)
{
  uint32_t count = 0;
  
  while(String[count] != '\0')
  {
    UART_SendChar(String[count]);
    count++;
  }
  UART_SendChar('\n');
}

uint8_t UART_ReceiveChar()
{
  uint8_t temp;
  UART0->C2 |= UART0_C2_RE(1u);          				//enable receiver
  
  while( (UART0->S1 & UART0_S1_RDRF_MASK) == 0);  		//wait data bufer full
  
  temp = UART0->D;              						//read data
  
  UART0->C2 &= ~UART0_C2_RE_MASK;       				//disable 
  
  return temp;
}

void ReceiverInterruptEnable()
{
  NVIC_EnableIRQ(UART0_IRQn);
  UART0->C2 |= UART0_C2_RIE(1u);
}

void ReceiverInterruptDisable()
{
  UART0->C2 &= ~UART0_C2_RIE_MASK;
}

