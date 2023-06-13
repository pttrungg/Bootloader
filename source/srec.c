#include "srec.h"

static Queue q;

uint8_t TextToInt (int8_t c)   /*convert char to hex*/
{
  uint8_t a;
  if(c>='0' && c<='9')
  {
    a=c-48;
  }
  if(c>='A' && c<='F')
  {
     a=c-55;
  }
  return a;
}

uint8_t LineSize (uint8_t data[])
{
  uint8_t i = 0;
  while (data[i] != '\n')
  {
      i++;
  }
  i++;
  return i;
}

void initQueue() 
{
  q.front = 0;
  q.rear = 0;
}



uint8_t isQueueEmpty() 
{
  return (q.front == q.rear);
}

uint8_t isQueueFull() 
{
  return ((q.rear + 1) % QUEUE_SIZE == q.front);
}

void enqueue(uint32_t address, uint32_t data) 
{
  if (isQueueFull()) 
  {
    return;				    // Queue is full
  }
  q.flashdata[q.rear].address = address;
  q.flashdata[q.rear].data = data;
  q.rear = (q.rear + 1) % QUEUE_SIZE;
}



FlashData dequeue() {
  FlashData value;
  if (isQueueEmpty())
  {
    value.address = 0;
    value.data = 0;
    // Queue is empty
    return value; // Or any other appropriate value
  }
    
  value = q.flashdata[q.front];
  q.front = (q.front + 1) % QUEUE_SIZE;
  return value;
}


uint8_t queueSize() 
{
  if (q.rear >= q.front) 
  {
    return q.rear - q.front;
  } 
  else 
  {
    return QUEUE_SIZE - (q.front - q.rear);
  }
}
