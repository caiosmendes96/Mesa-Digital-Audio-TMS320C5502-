# Mesa-Digital-Audio-TMS320C5502-
Projeto de reconstrução de efeitos de áudio de uma mesa digital desenvolvido na linguagem C para a disciplina ENGG54 da UFBA, ministrada pelo Prof. Tiago Trindade em 2024.2. Usamos a tela OLED e as interfaces de áudio (saída e entrada) na implementação em tempo real realizada no kit TMS320C5502 eZdsp, através da IDE Code Composer Studio.

##
<h3>Introdução</h3>

Este trabalho visa descrever a implementação do projeto semestral da matéria. Neste projeto, nós vamos reconstruir efeitos da mesa digital Teyun A8 no kit de desenvolvimento TMS320C5502 eZdsp:

<h6>Kit TMS320C5502 eZdsp</h6>

![image](https://github.com/user-attachments/assets/6d17b30c-d9fe-40d1-a85b-6f9a7bd0b6a4)

##

<h3>Desenvolvimento</h3>

O sinal será recebido por microfone, aplicado os efeitos e reproduzindo em alguma saída de áudio com tais efeitos. Tudo ocorrendo em tempo real.

![image](https://github.com/user-attachments/assets/9b91dc6e-38f0-401f-9be2-09bef322d704)


Todos os efeitos foram construídos na placa por meio de códigos em C, escritos e compilados na ferramenta Code Composer Studio.

Os efeitos que serão aplicados somam oito no total e replicam suas contrapartidas na mesa digital supracitada, podendo ser vistos a seguir:

<h4>Efeitos:</h4>

  - 01. REV-ROOM1
  - 02. REV-STAGE Ab
  - 03. REV-STAGE C
  - 04. REV-STAGE E
  - 05. REV-STAGE G
  - 06. CHORUS
  - 07. RADIO-VOICE 
  - 08. VOCAL

A placa também teve criada para ela um display que permite a seleção do efeito a ser aplicado, com os botões permitindo a mudança de efeito e seleção.
  
![image](https://github.com/user-attachments/assets/7621bdbb-b726-49d0-9724-443cc13be8b8)

##

<h3>Resultados</h3>

- 01. REV-ROOM1: 
    reverb com 500 amostras, delay ≈ 10.4 ms

- 02. REV-STAGE Ab: 
    reverb com 1024 amostras, delay ≈ 21.33 ms

- 03. REV-STAGE C: 
    reverb com 2048 amostras, delay ≈ 42.67 ms

- 04. REV-STAGE E:
    reverb com 3500 amostras, delay ≈  72.92 ms

- 05. REV-STAGE G: 
      reverb com 3500 amostras, delay ≈  85.33 ms

- 06. CHORUS: 
    reverb com 2048 amostras + pitch fator =  2.0; 4096/2 = 2048 amostras

- 07. RADIO-VOICE: 
reverb com 4000 amostras +  pitch fator =  1.5; 4096/1.5 =  2730 amostras

- 08. VOCAL: ganho = 0.7







