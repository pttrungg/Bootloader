#ifndef __SREC_H__
#define __SREC_H__

#include "MKL46Z4.h"

#define QUEUE_SIZE (256u)

typedef struct {
  uint32_t address;
  uint32_t data;
} FlashData;

typedef struct {
  FlashData flashdata[QUEUE_SIZE];
  uint8_t front;
  uint8_t rear;
} Queue;


uint8_t TextToInt (int8_t c);

uint8_t LineSize (uint8_t data[]);

void initQueue();

uint8_t isQueueEmpty();

uint8_t isQueueFull();

void enqueue(uint32_t address, uint32_t data);

FlashData dequeue();

uint8_t queueSize();

#endif