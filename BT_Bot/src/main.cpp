/*
  PROJETO: Robô controlado via bluetooth.
  Módulo Bluetooth HC-05
  Módulo Motor Driver L298N
  2x Caixa de redução com rodas e motor DC 6v
  APP Android: BT Car Controller-Arduino/ESP (Giristudio)
  Autor: Prof. Ronaldo Borges 
*/
#include <Arduino.h>
#include <SoftwareSerial.h>

#define BT_RX 9 // Arduino RX (conectar ao TX do HC-05)
#define BT_TX 10 // Arduino TX (conectar ao RX do HC-05) [opcional, pois não enviaremos dados ao HC-05]

SoftwareSerial BT(BT_RX, BT_TX); // cria objeto para comuicar com módulo bluetooth

// Pinos do Motor Direito (Canal A)
#define EN_D 3 // Velocidade (PWM)
#define IN1  4 // Direção 1
#define IN2  5 // Direção 2

// Pinos do Motor Esquerdo (Canal B)
#define EN_E 6 // Velocidade (PWM)
#define IN3  7 // Direção 1
#define IN4  8 // Direção 2

// Velocidade inicial
uint8_t velocidade = 64;
uint8_t compensacao = 15;  //o robô não está mecanicamente perfeito, por isso uma das todas é mais lenta. Esta variável será somada a velociadade da roda mais lenta.

void setup() {
  // Configurar pinos dos motores como saída (específicos)
  pinMode(EN_D, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN_E, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Inicializar comunicação serial USB e Bluetooth a 9600 bits por segundo.
  Serial.begin(9600);
  BT.begin(9600);

  // Parar motores no início
  parar();
}

void loop() {
  // Ler dados vindos do bluetooth
  if (BT.available() > 0) {
    char comando = BT.read();
    // Imprimir na serial USB o que veio do HC-05
    Serial.print("HC-05 -> ");
    Serial.println(comando);
    processarComando(comando);  // o comando é um único caractere vindo do aplicativo via bluetooth
  }
 
}

// ============ FUNÇÕES DE MOVIMENTO ============

void paraFrente() {
  // Motor Direito - Frente
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN_D, velocidade);
  
  // Motor Esquerdo - Frente
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_E, velocidade+compensacao);
}

void paraTras() {
  // Motor Direito - Trás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN_D, velocidade);
  
  // Motor Esquerdo - Trás
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_E, velocidade+compensacao);
}

void curvaAdireita() {
  // Motor Direito - Reduz velocidade (ou parado)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN_D, velocidade / 2);
  
  // Motor Esquerdo - Velocidade normal
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_E, velocidade+compensacao);
}

void curvaAesquerda() {
  // Motor Direito - Velocidade normal (avança mais)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN_D, velocidade);
  
  // Motor Esquerdo - Reduz velocidade (o robô vira para esquerda)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_E, velocidade / 2);
}

void curvaTraseEsquerda() {
  // Motor Direito - Trás com velocidade normal
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN_D, velocidade);
  
  // Motor Esquerdo - Trás com redução (o robô vira para esquerda recuando)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_E, velocidade / 2);
}

void curvaTrasDireita() {
  // Motor Direito - Trás com redução
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN_D, velocidade / 2);
  
  // Motor Esquerdo - Trás com velocidade normal (o robô vira para direita recuando)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_E, velocidade);
}

void curvaRotacaoEsquerda() {
  // Motor Direito - Frente
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN_D, velocidade);
  
  // Motor Esquerdo - Trás (rotação no lugar)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_E, velocidade);
}

void curvaRotacaoDireita() {
  // Motor Direito - Trás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN_D, velocidade);
  
  // Motor Esquerdo - Frente (rotação no lugar)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_E, velocidade);
}

void parar() {
  // Parar Motor Direito
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN_D, 0);
  
  // Parar Motor Esquerdo
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(EN_E, 0);
}

// ============ PROCESSAMENTO DE COMANDOS ============

void processarComando(char comando) {
  switch (comando) {
    case 'F':  // Frente
      paraFrente();
      break;
    case 'B':  // Trás
      paraTras();
      break;
    case 'G':  // Curva à Esquerda avançando
      curvaAesquerda();
      break;
    case 'H':  // Curva à Direita avançando
      curvaAdireita(); 
      break;
    case 'I':  // Curva à Esquerda recuando
      curvaTraseEsquerda();
      break;
    case 'J':  // Curva à Direita recuando
      curvaTrasDireita();
      break;
    case 'L':  // Rotação à Esquerda
      curvaRotacaoEsquerda();
      break;
    case 'R':  // Rotação à Direita
      curvaRotacaoDireita();
      break;
    case 'S':  // Parar
      parar();
      break;
    case '1':  // Aumentar velocidade
      velocidade = 32;
      break;
    case '2':  // Aumentar velocidade
      velocidade = 64;
      break;
    case '3':  // Aumentar velocidade
      velocidade = 96;
      break;
    case '4':  // Aumentar velocidade
      velocidade = 128;
      break;
    default:
      break;
  }
}