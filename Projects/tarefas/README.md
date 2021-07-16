# ELF74

Repositório destinado a prática 4 da disciplina de Sistemas Embarcados, turma ELF74, ministrada na Universidade Tecnológica Federal do Paraná (UTFPR). 

## Professor orientador

Hugo Vieira Neto

## Placa de desenvolvimento

[ARM® Cortex®-M4F-Based MCU TM4C1294 Connected LaunchPad™ Evaluation Kit](https://www.ti.com/tool/EK-TM4C1294XL)

## Analise e discussão da atividade
### Exercício prático 2

#### 2.1 Compilar o projeto "tarefas"

Criado um novo repositório para RTOS.

#### 2.2 Compilar o projeto "tarefas"

Dentro do arquivo rtx_lib.c foi possível encontrar as seguintes declarações de variáveis:
- (uint32_t)OS_TICK_FREQ
- (uint32_t)OS_THREAD_NUM
- (uint32_t)OS_THREAD_DEF_STACK_NUM

#### 2.3 Alteração de OS_TICK_FREQ
Inicialmente o valor de OS_TICK_FREQ era 1000 e após a alteração de seu valor para 500 o tempo em que o led demora para desligar e religar aumentou, ou seja, teve sua frequência reduzida.
#### 2.4 Retorno no valor de OS_TICK_FREQ
A variável OS_TICK_FREQ define uma base de tempo para delays e timeouts para as tasks (threads) que estiverem rodando no sistema. Um exemplo de conta para o período com base no  OS_TICK_FREQ seria:
<p xml align="center">
<a href="https://www.codecogs.com/eqnedit.php?latex=T&space;=\frac{1}{OS\,&space;TICK\,&space;FREQ}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?T&space;=\frac{1}{OS\,&space;TICK\,&space;FREQ}" title="T =\frac{1}{OS\; TICK\; FREQ}" /></a>
</p>
A thread1 apresenta um osDelay(N) como forma de controle temporal, sendo assim, a partir do momento em que é chamada a função o processamento vai ficar estacionado no mesmo ponto por N milissegundos.
Já thread2 apresenta um osDelayUntil(N + M) como forma de controle. A maior diferença entre as duas formas de "parar" o processamento um um determinado tempo é que desta forma como esta sendo aplicada na segunda thread o valor pode ser dinâmico, como no exemplo N+M com M sendo uma variável de entrada que pode-se alterar, e na primeira o valor deve ser estático. Vale notar que devido ao modo como está sendo atribuído o valor de N (tick) no código.

####  2.5-2.7 Retorno no valor de OS_TICK_FREQ
As alterações requisitadas estão dentro do diretório tarefas

## Referências
- The Definitive Guide to ARM® Cortex®-M3 and Cortex®-M4 Processors, Third Edition by Joseph Yiu