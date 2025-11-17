BT_Bot
=======

Projeto: Robô controlado via Bluetooth (HC-05) e driver L298N.

Visão geral
----------
Este projeto implementa controle básico de um robô com dois motores DC (duas rodas motrizes) usando um módulo Bluetooth HC-05 e um Arduino (PlatformIO). O app recomendado para controle é "BT Car Controller - Arduino/ESP (Giristudio)".

Principais recursos
- Comunicação com o HC-05 usando `SoftwareSerial` (pinos configuráveis)
- Comandos via Bluetooth para:
  - frente, atrás, curvas (avançando e recuando), rotação no lugar, stop
  - ajuste simples de velocidade com `+` / `-`
- Leituras do HC-05 impressas na Serial USB para depuração

Mapeamento de pinos (conforme `src/main.cpp`)
--------------------------------------------
- Motor Direito (canal A):
  - `EN_D` (PWM): `11`
  - `IN1`: `10`
  - `IN2`: `9`

- Motor Esquerdo (canal B):
  - `EN_E` (PWM): `5`
  - `IN3`: `6`
  - `IN4`: `7`

- Bluetooth (SoftwareSerial):
  - `BT_RX` (Arduino recebe do HC-05 TXD): `A3`
  - `BT_TX` (Arduino envia para HC-05 RXD): `A4`

- Pinos adicionais configurados na inicialização:
  - `A1` é configurado como `HIGH` (saída)
  - `A2` é configurado como `LOW` (saída)

Observações de hardware importantes
----------------------------------
- Conecte HC-05 TXD -> `A3` e HC-05 RXD <- `A4`.
- IMPORTANTE: o pino `RXD` do HC-05 é 3.3V tolerante. Sempre use um divisor de tensão (ou level shifter) no sinal Arduino TX -> HC-05 RX (Arduino `A4` -> HC-05 `RXD`). Um divisor simples com `R1 = 1k` e `R2 = 2k` (aprox.) é suficiente.
- `VCC` do HC-05 -> `5V`, `GND` -> `GND`.
- `EN` / `KEY` do HC-05: deixe desconectado para operação normal; conecte conforme a documentação para entrar em modo AT.

Comandos Bluetooth suportados
-----------------------------
- `F` : Frente (ambos motores frente)
- `B` : Trás (ambos motores ré)
- `L` : Curva à esquerda avançando (motor direito mais rápido)
- `R` : Curva à direita avançando (motor esquerdo mais rápido)
- `Q` : Curva à esquerda recuando (motor direito mais rápido para trás)
- `W` : Curva à direita recuando (motor esquerdo mais rápido para trás)
- `C` : Rotação à esquerda (motor direito frente, esquerdo trás)
- `D` : Rotação à direita (motor esquerdo frente, direito trás)
- `S` : Parar
- `+` : Aumentar velocidade
- `-` : Diminuir velocidade

Notas sobre comportamento do robô
---------------------------------
- O robô possui apenas duas rodas motrizes; movimentos "diagonais" não são fisicamente possíveis. Curvas são obtidas reduzindo a velocidade de um motor enquanto o outro segue com velocidade normal; rotações no lugar usam motores em sentido oposto.
- Valores PWM de velocidade estão definidos em `src/main.cpp` (variável `velocidade`, com limites `VELOCIDADE_MIN` e `VELOCIDADE_MAX`). Ajuste conforme necessário.

Build, Upload e Depuração (PlatformIO)
--------------------------------------
No terminal (PowerShell) dentro da pasta do projeto, para compilar:

```powershell
platformio run
# ou, se o PlatformIO não estiver no PATH:
# C:\Users\<seu_usuario>\.platformio\penv\Scripts\platformio.exe run
```

Para enviar para a placa:

```powershell
platformio run --target upload
# ou usando o executável completo do PlatformIO se necessário
```

Para abrir o monitor serial (9600 baud):

```powershell
platformio device monitor -b 9600
```

Testes e debug
--------------
- Abra o Monitor Serial; ao enviar comandos do app via HC-05 você verá mensagens do tipo:
  `HC-05 -> F`
- Também é possível enviar caracteres diretamente pelo Monitor Serial (útil para debug manual).

Alterações importantes no código
-------------------------------
- `SoftwareSerial` configurado com `BT_RX=A3` e `BT_TX=A4` para refletir a fiação do usuário.
- `setup()` força `A1` = HIGH e `A2` = LOW conforme solicitado.

Próximos passos sugeridos
-------------------------
- Verificar a fiação física e adicionar o divisor de tensão no TX do Arduino -> RX do HC-05.
- Fazer upload, abrir o monitor serial e testar os comandos do app.
- Se preferir usar pinos digitais 2/3 para `SoftwareSerial` (mais convencionais), posso atualizar o código e o README para essa opção.

Autor
-----
Prof. Ronaldo Borges


Licença
-------
Uso livre para fins didáticos. Modifique conforme necessário.
