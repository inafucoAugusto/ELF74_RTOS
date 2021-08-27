#include <stdbool.h>
#include <stdio.h>
#include "cmsis_os2.h" // CMSIS-RTOS

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "system_TM4C1294.h"

#define MSG_SIZE 30

// threads relacionadas a comunicação serial
osThreadId_t thread_read_serial_id, thread_write_serial_id;

// thread para controle dos elevadores
osThreadId_t thread_elevador;

// distancia em que o elevador deve estar 
int dist_andar[16] = {0, 5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000, 55000, 60000, 65000, 70000, 75000};

// fila de mensagens para cada elevador
static osMessageQueueId_t msgQueue1_s, msgQueue2_s, msgQueue3_s;
static osMessageQueueId_t msgQueue1_d, msgQueue2_d, msgQueue3_d;
static osMessageQueueId_t queue_destiny_e, queue_destiny_c, queue_destiny_d;

typedef struct{
  char elevador;
  int andar;
  int current_dist;
  char estado[30];
  osMessageQueueId_t msgQueue;
  char destiny[30];
}Elevador;

Elevador elevador_e, elevador_c, elevador_d;

void read_serial(void *arg){
  char msg[MSG_SIZE];
  int a;
  while(1){
    a = UARTgets(msg, MSG_SIZE);
    if(msg != ""){
      if(a == 2){}

      else if(a == 3){}

      else if(a == 4){
        // elevador 1
        if(msg[0] == 'e'){
          if(msg[4] == 'd'){
            osMessageQueuePut(msgQueue1_d, msg, NULL, osWaitForever);
          }
          else{
            osMessageQueuePut(msgQueue1_s, msg, NULL, osWaitForever);
          }
        }

        //elevador 2
        if(msg[0] == 'c'){
          if(msg[4] == 'd'){
            osMessageQueuePut(msgQueue2_d, msg, NULL, osWaitForever);
          }
          else{
            osMessageQueuePut(msgQueue2_s, msg, NULL, osWaitForever);
          }
        }

        //elevador 3
        if(msg[0] == 'd'){
          if(msg[4] == 'd'){
            osMessageQueuePut(msgQueue3_d, msg, NULL, osWaitForever);
          }
          else{
            osMessageQueuePut(msgQueue3_s, msg, NULL, osWaitForever);
          }
        }
      }
    }
  }
}

void write_serial(void *arg){
  char msg[MSG_SIZE];
  while(1){
    osMessageQueueGet(msgQueue1_s, msg, NULL, osWaitForever);
    UARTprintf("%s\n", msg);
  }
}

// threads para consumir mensagens dos botes pressionados dentro 
void set_elevator_destiny1(void *arg){
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    osMessageQueueGet(queue_destiny_e, elevador_e.destiny, NULL, osWaitForever);
    //UARTprintf("%s\n", msg);
  }
}

void set_elevator_destiny2(void *arg){
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    osMessageQueueGet(queue_destiny_c, elevador_c.destiny, NULL, osWaitForever);
    //UARTprintf("%s\n", msg);
  }
}

void set_elevator_destiny3(void *arg){
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    osMessageQueueGet(queue_destiny_d, elevador_d.destiny, NULL, osWaitForever);
    //UARTprintf("%s\n", msg);
  }
}

extern void UARTStdioIntHandler(void);

void UARTInit(void){
  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));

  // Initialize the UART for console I/O.
  UARTStdioConfig(0, 115200, SystemCoreClock);

  // Enable the GPIO Peripheral used by the UART.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

  // Configure GPIO Pins for UART mode.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
} // UARTInit

void UART0_Handler(void){
  UARTStdioIntHandler();
} // UART0_Handler


void main(void){
  // inicializacao do sistema
  osKernelInitialize();
  
  // inicializacao das queues
  UARTInit();
  msgQueue1_s = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  msgQueue2_s = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  msgQueue3_s = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  msgQueue1_d = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  msgQueue2_d = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  msgQueue3_d = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  
  queue_destiny_e = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  queue_destiny_c = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  queue_destiny_d = osMessageQueueNew(60U, sizeof(char[MSG_SIZE]), NULL);
  
  // inicializacao das threads
  thread_read_serial_id = osThreadNew(read_serial, NULL, NULL);
  thread_write_serial_id = osThreadNew(write_serial, NULL, NULL);
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1){};
  
} // main