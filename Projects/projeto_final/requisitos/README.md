# ELF74

Repositório destinado ao projeto final da disciplina de Sistemas Embarcados, turma ELF74, ministrada na Universidade Tecnológica Federal do Paraná (UTFPR). 

## Professor orientador

Hugo Vieira Neto
Douglas Renaux
Andre Schneider de Oliveira

## Placa de desenvolvimento

[ARM® Cortex®-M4F-Based MCU TM4C1294 Connected LaunchPad™ Evaluation Kit](https://www.ti.com/tool/EK-TM4C1294XL)

## Requisitos Funcionais e Não Funcionais - Sistema com três elevadores

### Requisitos Funcionais
- Os elevadores devem funcionar de forma independente.
- Se em um mesmo elevador houver uma requisição de subida e outra de descida, deve-se executar a que teve a requisição primeira.
- O elevador não pode passar do andar 15.
- O elevador não pode descer além do andar zero.
- O elevador mesmo sem nenhuma ordem, ou seja, quando estiver parado deve permanecer com a porta fechada.
- Enquando se movimenta o elevador deve parmanecer com a porta fechada.
- Após parar em um andar o elevador deve permanecer com a porta fechada por pelo menos 2 segundos antes de abrí-la.
- Ao chegar no andar de destino o elevador deverá ficar com a porta aberta por pelo menos 10 segundos.
- O elevador deve ficar parado, último andar de destino, até que hava uma nova requisição.
- As luzes internas (botões) do elevador devem ficar desligadas enquanto não forem pressionadas
- Ao se pressionar um botão sua luz deve acender.
- A luz, uma vez acesa, só deve voltar a estar deligada quando sua funcionalidade chegar ao fim; chegar no destino final, por exemplo.
- O elevador deverá funcionar de forma alternada, isto é, uma vez realizando trajetórias de subidas e na sequência de descida.
- Se o elevador for realizar uma trajetória de descida o mesmo deve começar do andar mais alto requisitado.
- Se o elevador for realizar uma trajetória de subida o mesmo deve começar do andar mais baixo requisitado.


### Requisitos Não Funcionais
- Código desenvolvido em linguagem C.
- Baud rate de comunicação serial entre interface gráfica e placa de desenvolvimento de 115200 bps.
- Sistema de operação em tempo real Keil RTX5.
- Data limite de entrega 26 de Agosto de 2021.
- Entrega de vídeo comprovando o funcionamento do projeto.

## Referências
- The Definitive Guide to ARM® Cortex®-M3 and Cortex®-M4 Processors, Third Edition by Joseph Yiu