#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS
#include "driverbuttons.h"

osThreadId_t thread1_id, thread2_id, thread3_id, thread4_id, throw_flag, control_led, block_control, in_led_selection;
uint8_t taget_led = 1;
osMutexId_t mutex_id;
uint32_t total_cicle = 10;
uint8_t bla = 0;

typedef struct{
  uint8_t led;
  int duty_cicle;
  int thread_id;
}LED;

LED led1, led2, led3, led4;

void GPIOJ_Handler(void){
  if(ButtonRead(USW1) == 0){
    ButtonIntClear(USW1);
    ButtonIntClear(USW2);
    osThreadFlagsSet(control_led, 0x0001);
  }
  if(ButtonRead(USW2) == 0){
    ButtonIntClear(USW1);
    ButtonIntClear(USW2);
    osThreadFlagsSet(in_led_selection, 0x0001);
  }
}

const osMutexAttr_t Phases_Mutex_attr = {
  "PhasesMutex",                            // human readable mutex name
  osMutexRecursive | osMutexPrioInherit,    // attr_bits
  NULL,                                     // memory for control block   
  0U                                        // size for control block
  };

void thread_led(void *arg){
  uint8_t state = 0;
  LED *target_led = arg;
  while(1){
    osThreadFlagsWait((*target_led).thread_id, osFlagsWaitAny, osWaitForever);
    state ^= (*target_led).led;
    LEDWrite((*target_led).led, state);
    osDelay((*target_led).duty_cicle);

    state ^= (*target_led).led;
    LEDWrite((*target_led).led, state);
    osDelay(10 - (*target_led).duty_cicle);
  }
}

void thread_throw_flag(void *arg){
  while(1){
    osMutexAcquire(mutex_id, osWaitForever);
    osThreadFlagsSet(thread1_id, 0x0001);
    osThreadFlagsSet(thread2_id, 0x0002);
    osThreadFlagsSet(thread3_id, 0x0003);
    osThreadFlagsSet(thread4_id, 0x0004);
    osMutexRelease(mutex_id);
  }
}

void thread_control_led(void *arg){
//  LED led_position[3] = {led1, led2, led3, led4}
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    osMutexAcquire(mutex_id, osWaitForever);

    if(taget_led == 1){
      if(led1.duty_cicle == 10){
        led1.duty_cicle = 1;
      }
      else{
        led1.duty_cicle = led1.duty_cicle + 1;
      }
    }

    else if(taget_led == 2){
      if(led2.duty_cicle == 10){
        led2.duty_cicle = 1;
      }
      else{
        led2.duty_cicle = led2.duty_cicle + 1;
      }
    }

    else if(taget_led == 3){
      if(led3.duty_cicle == 10){
        led3.duty_cicle = 1;
      }
      else{
        led3.duty_cicle = led3.duty_cicle + 1;
      }
    }

    else if(taget_led == 4){
      if(led4.duty_cicle == 10){
        led4.duty_cicle = 1;
      }
      else{
        led4.duty_cicle = led4.duty_cicle + 1;
      }
    }
    osMutexRelease(mutex_id);
  }
}

void thread_block(void *arg){
  uint8_t aux = 1;
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    if(aux){
      osMutexAcquire(mutex_id, osWaitForever);
      aux = !aux;
      osDelay(10);
      osThreadFlagsSet(control_led, 0x0001);
    }
    else{
      osMutexRelease(mutex_id);
      aux = !aux;
    }
  }
}

void thread_in_led_selection(void *arg){
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    osMutexAcquire(mutex_id, osWaitForever);
    if(taget_led < 4){
      taget_led = taget_led + 1;
    }
    else{
      taget_led = 1;
    }
    osDelay(10);
    osMutexRelease(mutex_id);
  }
}

void main(void){
  ButtonInit(USW1);
  ButtonIntEnable(USW1);
  ButtonInit(USW2);
  ButtonIntEnable(USW2);
  
  LEDInit(LED1 | LED2 | LED3 | LED4);
  
  led1.led = LED1;
  led2.led = LED2;
  led3.led = LED3;
  led4.led = LED4;

  led1.duty_cicle = 1;
  led2.duty_cicle = 3;
  led3.duty_cicle = 5;
  led4.duty_cicle = 7;

  led1.thread_id = 0x0001;
  led2.thread_id = 0x0002;
  led3.thread_id = 0x0003;
  led4.thread_id = 0x0004;

  osKernelInitialize();

  thread1_id = osThreadNew(thread_led, &led1, NULL);
  thread2_id = osThreadNew(thread_led, &led2, NULL);
  thread3_id = osThreadNew(thread_led, &led3, NULL);
  thread4_id = osThreadNew(thread_led, &led4, NULL);
  throw_flag = osThreadNew(thread_throw_flag, NULL, NULL);
  control_led = osThreadNew(thread_control_led, NULL, NULL);
  in_led_selection = osThreadNew(thread_in_led_selection, NULL, NULL);
  block_control = osThreadNew(thread_block, NULL, NULL);
  
  mutex_id = osMutexNew(&Phases_Mutex_attr);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1){};
} // main