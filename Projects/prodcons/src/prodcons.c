#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS
#include "driverbuttons.h"
#define BUFFER_SIZE 8

osThreadId_t produtor_id, consumidor_id;
osSemaphoreId_t vazio_id, cheio_id;
uint8_t buffer[BUFFER_SIZE];
uint8_t global_index_i = 0;
uint8_t global_count = 0;


//osStatus_t {
//  osOK = 0,
//  osError = -1,
//  osErrorTimeout = -2,
//  osErrorResource = -3,
//  osErrorParameter = -4,
//  osErrorNoMemory = -5,
//  osErrorISR = -6,
//  osStatusReserved = 0x7FFFFFFF
//}

void GPIOJ_Handler(void){
  ButtonIntClear(USW1);
  if(osSemaphoreAcquire(vazio_id, 0) == osOK){
    //osSemaphoreAcquire(vazio_id, 0);
    buffer[global_index_i] = global_count;      // coloca no buffer
    global_index_i++; // incrementa �ndice de coloca��o no buffer
    if(global_index_i >= BUFFER_SIZE){
        global_index_i = 0;
    }
    global_count++;
    global_count &= 0x0F; // produz nova informa��o
    osSemaphoreRelease(cheio_id);
  }
}

void produtor(void *arg){
  uint8_t index_i = 0, count = 0;
  while(1){
    osSemaphoreAcquire(vazio_id, osWaitForever); // h� espa�o dispon�vel?
    buffer[index_i] = count;      // coloca no buffer
    osSemaphoreRelease(cheio_id); // sinaliza um espa�o a menos
    
    index_i++; // incrementa �ndice de coloca��o no buffer
    if(index_i >= BUFFER_SIZE)
      index_i = 0;
    
    count++;
    count &= 0x0F; // produz nova informa��o
    osDelay(500);
  } // while
} // produtor

void consumidor(void *arg){
  uint8_t index_o = 0, state;
  while(1){
    osSemaphoreAcquire(cheio_id, osWaitForever); // h� dado dispon�vel?
    state = buffer[index_o];                     // retira do buffer
    osSemaphoreRelease(vazio_id);                // sinaliza um espa�o a mais
    index_o++;
    if(index_o >= BUFFER_SIZE)                   // incrementa �ndice de retirada do buffer
      index_o = 0;
    LEDWrite(LED4 | LED3 | LED2 | LED1, state); // apresenta informa��o consumida
    osDelay(500);
  } // while
} // consumidor

void main(void){
  SystemInit();
  LEDInit(LED4 | LED3 | LED2 | LED1);
  
  ButtonInit(USW1);
  ButtonIntEnable(USW1);
  ButtonInit(USW2);
  ButtonIntEnable(USW2);
  
  //SysTickPeriodSet(12000000); // f = 1Hz para clock = 24MHz
  //SysTickIntEnable();
  //SysTickEnable();

  osKernelInitialize();

  consumidor_id = osThreadNew(consumidor, NULL, NULL);
  produtor_id = osThreadNew(produtor, NULL, NULL);
  

  vazio_id = osSemaphoreNew(BUFFER_SIZE, BUFFER_SIZE, NULL); // espa�os dispon�veis = BUFFER_SIZE
  cheio_id = osSemaphoreNew(BUFFER_SIZE, 0, NULL);           // espa�os ocupados = 0
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1){};
} // main
