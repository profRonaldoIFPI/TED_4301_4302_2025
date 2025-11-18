/*
  PROJETO: SEGUIDOR DE LINHAS BÁSICO.
  Sensores: 2x Polulu QTR-1A Reflectance Sensor
  Atuadores: 
    Módulo Motor Driver L298N
    2x Caixa de redução com rodas e motor DC 6v
  Autor: Prof. Ronaldo Borges
*/
#include <Arduino.h>

#define EN_D 3  // Velocidade da direita (canal A)
#define IN1 5   // direção do motor A
#define IN2 4   // direção do motor A

#define EN_E 6  // Velocidade da esquerda (canal B)
#define IN3 8   // direção do motor B
#define IN4 7   // direção do motor B

#define SENSOR_DIR A0
#define SENSOR_ESQ A1
/* 
  Considerando uma bateria de 12v e motores de 6v,
  é bom respeitar o limite de tensão dos motores para que não queimem, 
  por isso a velocidade máxima 128 (50% de 255).
  Ajuste conforme a tensão da bateria.
*/
const int velocidade = 90;  // Velocidade máxima dos motores
int compensacaoEsq = -5;    // Compensação para o motor mais lento (+ = esquedo, - = direito)
/* 
  Limiar para detecção da linha.
  Ajustar conforme luz ambiente, cor da superficie ou distancia dos sensores em relação à superfície.
*/
#define limiar 300  // branco < limiar < preto

void setup() {

  for (int porta = 2; porta <= 8; ++porta) {
    pinMode(porta, OUTPUT);
  }

  pinMode(SENSOR_DIR, INPUT);
  pinMode(SENSOR_ESQ, INPUT);

  //sentido de rotação para frente em ambos os motores
  analogWrite(EN_D, velocidade);  // Velocidade do motor direito
  analogWrite(EN_E, velocidade + compensacaoEsq);

  // Serial.begin(9600);
}

void loop() {
  int valorSensorDir = analogRead(SENSOR_DIR);
  int valorSensorEsq = analogRead(SENSOR_ESQ);

  // Serial.print(valorSensorEsq);
  // Serial.print("< Esquerda | Direita >");
  // Serial.println(valorSensorDir);
  // delay(500);

  // Condições para seguir a linha
  if (valorSensorDir < limiar && valorSensorEsq < limiar) {
    digitalWrite(IN1, HIGH);  //direita
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);  //esquerda
    digitalWrite(IN4, LOW);
  }
  if (valorSensorDir < limiar && valorSensorEsq >= limiar) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  if (valorSensorDir >= limiar && valorSensorEsq < limiar) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  if (valorSensorDir >= limiar && valorSensorEsq > limiar) {
    analogWrite(EN_D, 0);  // Velocidade do motor direito
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    analogWrite(EN_E, 0);  // Velocidade do motor esquerdo
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
  } else {
    analogWrite(EN_D, velocidade);
    analogWrite(EN_E, velocidade + compensacaoEsq);
  }
}