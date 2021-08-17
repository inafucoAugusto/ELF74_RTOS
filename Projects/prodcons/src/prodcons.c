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
    global_index_i++; // incrementa índice de colocação no buffer
    if(global_index_i >= BUFFER_SIZE){
        global_index_i = 0;
    }
    global_count++;
    global_count &= 0x0F; // produz nova informação
    osSemaphoreRelease(cheio_id);
  }
}

void produtor(void *arg){
  uint8_t index_i = 0, count = 0;
  while(1){
    osSemaphoreAcquire(vazio_id, osWaitForever); // há espaço disponível?
    buffer[index_i] = count;      // coloca no buffer
    osSemaphoreRelease(cheio_id); // sinaliza um espaço a menos
    
    index_i++; // incrementa índice de colocação no buffer
    if(index_i >= BUFFER_SIZE)
      index_i = 0;
    
    count++;
    count &= 0x0F; // produz nova informação
    osDelay(500);
  } // while
} // produtor

void consumidor(void *arg){
  uint8_t index_o = 0, state;
  while(1){
    osSemaphoreAcquire(cheio_id, osWaitForever); // há dado disponível?
    state = buffer[index_o];                     // retira do buffer
    osSemaphoreRelease(vazio_id);                // sinaliza um espaço a mais
    index_o++;
    if(index_o >= BUFFER_SIZE)                   // incrementa índice de retirada do buffer
      index_o = 0;
    LEDWrite(LED4 | LED3 | LED2 | LED1, state); // apresenta informação consumida
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
  

  vazio_id = osSemaphoreNew(BUFFER_SIZE, BUFFER_SIZE, NULL); // espaços disponíveis = BUFFER_SIZE
  cheio_id = osSemaphoreNew(BUFFER_SIZE, 0, NULL);           // espaços ocupados = 0
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1){};
} // main
