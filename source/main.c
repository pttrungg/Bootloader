#include "MKL46Z4.h"
#include "middleware.h"
#include "uart.h"
#include "queue.h"
#include "flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_VECTOR_TABLE_OFFSET_FM1 (0x1400U)
#define APP_VECTOR_TABLE_OFFSET_FM2 (0x2800U)

uint8_t F_StartSREC = 0, F_EndSREC = 0 , F_Record = 0, F_StartByteCount = 0, F_StartAddress = 0, F_AddressByte = 0, F_StartData = 0;
uint8_t next_address = 0, f_byte = 0;
uint32_t Data_Address = 0, Data = 0;
uint16_t Bytecount = 0;
volatile uint8_t Flag_button = 0;

/*******************************************************************************
 * Functions
 ******************************************************************************/

void BootHandle_JumpToApp(uint32_t Address)
{
  if(Flash_Read(Address) == 0xFFFFFFFF)
  {
    UART_SendString("no program in this memory");
    return;
  }
  
  __disable_irq();

  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  
  SCB->VTOR = (uint32_t)Address;
  __set_MSP(*(uint32_t *)(Address));
  ((void (*)(void))(*(uint32_t *)(Address + 4U)))(); 
}


void PORTC_PORTD_IRQHandler()
{
  if(PORTC->ISFR == 0x1000)
  {
    Flag_button = 1;
    PORTC->ISFR |= (1<<12);
  }
}


void UART0_IRQHandler()
{
  if( (UART0->S1 & UART0_S1_RDRF_MASK) != 0)
  {
    uint8_t ReceiveData;
    ReceiveData = UART0->D;
    
    if (ReceiveData == 'S')
    {
        F_Record = 1;
    }
    else if (F_Record == 1)
    {
        if (ReceiveData == '0')    // S0
        {
            F_StartSREC = 1;			//confirm is SREC
        }
        else if (ReceiveData == '1')   // S1
        {
            F_StartByteCount = 1;
            F_AddressByte = 2;
        }
        else if (ReceiveData == '2')   // S2
        {
            F_StartByteCount = 1;
            F_AddressByte = 3;
        }
        else if (ReceiveData == '3')   // S3
        {
            F_StartByteCount = 1;
            F_AddressByte = 4;
        }
        else if(ReceiveData == '7' || ReceiveData == '8' || ReceiveData == '9')
        {
          F_EndSREC = 1; 
          ReceiverInterruptDisable();
        }
        F_Record = 0;
    }
    
    else if (F_StartByteCount == 1)
    {
        Bytecount = TextToInt(ReceiveData) << 4;
        F_StartByteCount = 2;
    }
    else if (F_StartByteCount == 2)
    {
        Bytecount += TextToInt(ReceiveData);
        F_StartByteCount = 0;
        F_StartAddress = 1;
    }
    
    else if (F_StartAddress != 0)
    {
        Data_Address += ((TextToInt(ReceiveData)) << ((F_AddressByte << 3) - (F_StartAddress << 2)));
        F_StartAddress++;
        
        if (F_StartAddress == (F_AddressByte << 1) + 1)
        {
            F_StartAddress = 0;
            F_StartData = 1;  
            Bytecount = (Bytecount - F_AddressByte - 1) << 1;                   // 1 = checksum
        }
    }
    else if (F_StartData != 0)
    {
        Data += ((TextToInt(ReceiveData)) << (32 - (F_StartData<<2)));
        
        F_StartData ++;
        f_byte++;
              
	if(f_byte >= Bytecount )	//end line srec
	{
          if(F_StartData < 9)
          {
            Data = ((Data >> 16) | 0xffff0000);
          } 
          enqueue(Data_Address + next_address, Data );          
          F_StartData = 0;
          Data_Address = 0;
          next_address = 0;
          Data = 0;
          Bytecount = 0;
          f_byte = 0;
	}
        
        if(F_StartData == 9 )
        {
            F_StartData = 1;
            Data_Address += next_address;
            enqueue(Data_Address, Data);        /*add DATA to queue & clear = 0*/
            Data = 0;
            next_address = 4;                  //add data + (address + 4) 
	}
    }
  }
  
  if( (UART0->S1 & UART0_S1_OR_MASK) != 0)
  {
    ReceiverInterruptDisable();
  }
}

void DelayCheck()
{
  SysTick->VAL = 0;
  SysTick->CTRL |= (1 << 0);
  while( ((SysTick->CTRL & (1 << 16)) == 0) && (Flag_button == 0));
  SysTick->CTRL &= ~(1 << 0);
}

/*******************************************************************************
 * Main
 ******************************************************************************/
void main()
{
  BTN2_Init();
  BTN2_Interrupt_Config();
  UART0_Init();
  ReceiverInterruptEnable();
  
  UART_SendString(".....Bootloader......");
  UART_SendString("-> wait 5s -> Main application");
  UART_SendString("-> Click button -> Backup application");
  
  Systick_Init(15000u);  
  DelayCheck();
  
  UART_SendString("----Click reset button -> back to bootloader----");
  if(Flag_button == 1)
  {
    UART_SendString("Application 2: send 0x55 to read temperature sensor");
    BootHandle_JumpToApp(APP_VECTOR_TABLE_OFFSET_FM2);
  }
  else
  {
    UART_SendString("Application 1: toggle led & button");
    BootHandle_JumpToApp(APP_VECTOR_TABLE_OFFSET_FM1);
  }
  
  if(Flash_Read(APP_VECTOR_TABLE_OFFSET_FM1) == 0xFFFFFFFF)
  {
    Flash_EraseSector(APP_VECTOR_TABLE_OFFSET_FM1, 5U);
    UART_SendString("send srec file by hercules app and reset program");
  }
  else if(Flash_Read(APP_VECTOR_TABLE_OFFSET_FM2) == 0xFFFFFFFF)
  {
    Flash_EraseSector(APP_VECTOR_TABLE_OFFSET_FM2, 10U);
    UART_SendString("send srec file by hercules app and reset program");
  }
  
  while(1)
  {   
    while(isQueueEmpty());
    FlashData a = dequeue();    
    Flash_Write(a.address, a.data); 
  }
}

