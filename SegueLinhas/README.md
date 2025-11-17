# SegueLinhas

Projeto de um robô seguidor de linhas básico desenvolvido em C++ para Arduino UNO utilizando PlatformIO. O firmware controla dois motores DC via driver L298N e lê dois sensores refletivos QTR-1A (Pololu) para manter o robô alinhado sobre uma faixa contrastante.

## Visão geral

- Ajusta uma velocidade máxima (`velocidade = 128`) para respeitar motores de 6 V alimentados por bateria de 12 V.  
- Usa um limiar (`limiar = 300`) para identificar quando os sensores estão sobre preto ou branco; valores devem ser calibrados conforme o piso e a iluminação.  
- Mantém a movimentação reta quando ambos sensores detectam a branco (supondo que a linha esteja entre os sensores).
- Reduz seletivamente a velocidade de um dos motores para corrigir desvios para quando detecta a linha.

## Hardware esperado

- Arduino Uno
- 2 × sensores refletivos Pololu QTR-1A (A0 e A1)
- Driver L298N (pinos digitais 3–8)
- 2 × motores DC com caixa de redução
- Bateria 12 V 

## Estrutura do projeto

```
.
├── include/          # Cabeçalhos adicionais (vazio por padrão)
├── lib/              # Bibliotecas locais (vazio)
├── src/main.cpp      # Lógica do seguidor de linha
├── test/             # Testes unitários (não utilizados)
└── platformio.ini    # Configuração PlatformIO (env: uno)
```

## Como compilar e fazer upload

1. Instale o [PlatformIO CLI](https://platformio.org/install/cli) ou use a extensão PlatformIO IDE no VS Code ou Use o Arduino IDE.  
2. Conecte o Arduino Uno e identifique a porta serial (ex.: `COM3` no Windows, `/dev/ttyACM0` no Linux).  
3. No terminal, execute:

```bash
pio run -t upload
```

Para apenas compilar sem subir ao hardware:

```bash
pio run
```

## Calibração e ajustes

- **Velocidade (`velocidade`)**: aumente ou reduza conforme a relação bateria/motor.  
- **Limiar (`limiar`)**: use `Serial.print` já presente no código para observar leituras e ajustar ao contraste do percurso.  
- **Correções**: para curvas mais suaves, altere o fator aplicado quando apenas um sensor enxerga a linha (atualmente `velocidade/2`).

