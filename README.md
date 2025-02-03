# Temporizador de um Disparo (One Shot)

## Descrição da Atividade
Esta atividade tem como objetivo projetar um sistema de temporização para o acionamento de LEDs utilizando a função `add_alarm_in_ms()` do Pico SDK. O sistema deve ser acionado por um botão (pushbutton) e seguir uma sequência de temporização predefinida para alterar o estado dos LEDs.

## Componentes Utilizados
- **Microcontrolador:** Raspberry Pi Pico W
- **LEDs:** 3 LEDs (azul, vermelho e verde)
- **Resistores:** 3 resistores de 330 Ω
- **Botão:** Pushbutton

## Requisitos do Sistema
1. **Acionamento dos LEDs:**
   - Quando o usuário pressionar o botão, todos os três LEDs serão ligados simultaneamente (estado alto).
   - Após um atraso de 3 segundos, um dos LEDs será desligado.
   - Após mais 3 segundos, um segundo LED será desligado, restando apenas um ligado.
   - Após mais 3 segundos, o último LED será desligado.
2. **Configuração do Temporizador:**
   - O temporizador deve ser ajustado para um atraso de 3.000ms (3 segundos) entre as mudanças de estado dos LEDs.
   - A mudança de estado dos LEDs deve ser implementada por meio de funções de callback do temporizador (exemplo: `turn_off_callback()`).
3. **Controle do Botão:**
   - O botão só pode ser pressionado novamente quando o último LED for desligado.
   - Durante a execução da rotina de temporização, o botão deve ser desativado para evitar uma nova chamada da função callback.
4. **Simulação na Ferramenta BitDogLab:**
   - Utilizar os GPIOs 11, 12 e 13 para os LEDs RGB.
   - O botão A deve ser configurado no GPIO 05.
5. **(Opcional) Software Debounce:**
   - Implementar uma rotina para atenuar o efeito de bouncing do botão.

## Exemplo de Fluxo do Sistema
1. O usuário pressiona o botão.
2. Todos os LEDs são ligados.
3. Após 3 segundos, um LED é desligado.
4. Após mais 3 segundos, um segundo LED é desligado.
5. Após mais 3 segundos, o último LED é desligado.
6. O sistema aguarda um novo acionamento do botão.

