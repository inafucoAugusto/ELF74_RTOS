# ELF74

Repositório destinado a prática 5 da disciplina de Sistemas Embarcados, turma ELF74, ministrada na Universidade Tecnológica Federal do Paraná (UTFPR). 

## Professor orientador

Hugo Vieira Neto

## Placa de desenvolvimento

[ARM® Cortex®-M4F-Based MCU TM4C1294 Connected LaunchPad™ Evaluation Kit](https://www.ti.com/tool/EK-TM4C1294XL)

## Analise e discussão da atividade
### Exercício prático 5

#### 5.6 ISR com Task
Dentro do projeto  [prodcons](https://github.com/inafucoAugusto/ELF74_RTOS/blob/main/Projects/prodcons/src/prodcons.c) foi adiciona uma ISR ao script. 
Após a alteração o consumidor consegue somente acessar o buffer se a ISR liberar o semáforo osSemaphoreRelease(cheio_id).
O maior ponto de destaque é o que acontece logo no início da ISR. Como uma interrupção não pode ficar "se enrolando" - deve ser executada o mais rápida possível - foi utilizada a função osSemaphoreAcquire(vazio_id, 0). Essa função tenta "pegar" o semáforo, sem causar nenhum tipo de delay ou retry, e o retorno dela é baseado na enum:

    osStatus_t {
      osOK = 0,
      osError = -1,
      osErrorTimeout = -2,
      osErrorResource = -3,
      osErrorParameter = -4,
      osErrorNoMemory = -5,
      osErrorISR = -6,
      osStatusReserved = 0x7FFFFFFF
    }

Logo, se não for possível a aquisição do sinal do semáforo o sinal do semáforo  cheio_id não será liberado para a task consumidor. 

## Referências
- The Definitive Guide to ARM® Cortex®-M3 and Cortex®-M4 Processors, Third Edition by Joseph Yiu
- https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__SemaphoreMgmt.html#ga7e94c8b242a0c81f2cc79ec22895c87b
- https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__Definitions.html#ga6c0dbe6069e4e7f47bb4cd32ae2b813e
