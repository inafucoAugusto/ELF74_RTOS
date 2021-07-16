#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

osThreadId_t thread1_id, thread2_id, thread3_id, thread4_id;

typedef struct{
  uint8_t led;
  int led_delay;
}LED;

void thread_led(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  LED *target_led = arg;
  while(1){
    tick = osKernelGetTickCount();
    state ^= (*target_led).led;
    LEDWrite((*target_led).led, state);
    osDelayUntil(tick + (*target_led).led_delay);
  } // while
} // thread2

void thread1(void *arg){
  uint8_t state = 0;
  while(1){
    state ^= LED1;
    LEDWrite(LED1, state);
    osDelay(100);                               // delay bruto; para no momento em que é chamado
  } // while
} // thread1

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    tick = osKernelGetTickCount();
    state ^= LED2;
    LEDWrite(LED2, state);
    
    osDelayUntil(tick + 100);                   // da um delay de 100 + tick; tick eh o valor que está no kernel 
                                                // no mesmo ponto em que a função osKernelGetTickCount é chamada
  } // while
} // thread2

void main(void){
  LEDInit(LED1 | LED2 | LED3 | LED4);
  LED led1, led2, led3, led4;
  
  led1.led = LED1;
  led2.led = LED2;
  led3.led = LED3;
  led4.led = LED4;
  
  led1.led_delay = 200;
  led2.led_delay = 300;
  led3.led_delay = 500;
  led4.led_delay = 700;
  
  osKernelInitialize();

  thread1_id = osThreadNew(thread_led, &led1, NULL);
  thread2_id = osThreadNew(thread_led, &led2, NULL);
  thread3_id = osThreadNew(thread_led, &led3, NULL);
  thread4_id = osThreadNew(thread_led, &led4, NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
