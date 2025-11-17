#include <Arduino.h> //exclua se usar o Arduino IDE
#include <Servo.h> 
/* display de 7 segmentos
   a 
f     b
   g
e     c
   d
*/
int digitos[10][7] = 
{ // a,b,c,d,e,f,g
    {0,0,0,0,0,0,1},  //0 abcdef
    {1,0,0,1,1,1,1},  //1 ab
    {0,0,1,0,0,1,0},  //2 abdeg
    {0,0,0,0,1,1,0},  //3 abdcg
    {1,0,0,1,1,0,0},  //4 bcfg
    {0,1,0,0,1,0,0},  //5 acdfg
    {0,1,0,0,0,0,0},  //6 acdef
    {0,0,0,1,1,1,1},  //7 abc
    {0,0,0,0,0,0,0},  //8 abcdefg
    {0,0,0,0,1,0,0},  //9 abc
};
int portas[7] = {2,3,4,5,6,7,8};

Servo servo;

#define SERVO 10
#define BUZZER 9

void mostraDigito(int digito);

void setup() {
  servo.attach(SERVO);
  servo.write(0); // 0~180 
  pinMode(BUZZER, OUTPUT);
  for(int i=2; i<=8; i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
  }
  pinMode(A0, INPUT_PULLUP); //HIGH
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
} 
int digito = 9, tempoAtual, tempoAnterior = 0, intervalo = 1000;
bool buzz = false;

void loop() {
  tempoAtual = millis(); //tempo que o arduino está ligado
  if(tempoAtual - tempoAnterior >= intervalo){ //entra aqui a cada intervalo 
    tempoAnterior = tempoAtual;
    mostraDigito(digito); //0~9
    digito--;//-1
    if(buzz) tone(BUZZER,300,100); //beep
  }
  if((digito==-1)||(!digitalRead(A0)){ //estoura 
    servo.write(180); //a testar o grau suficiente
    tone(BUZZER,1000,3000);
  }
  if(!digitalRead(A1)){ //conta mais rápido
    intervalo = 500;
  }
  if(!digitalRead(A2)){  //volta o tempo
    digito = 9;
  }
  if(!digitalRead(A3)){  //liga o buzzer
    buzz = true;
  }
  if(!digitalRead(A4)){  //desarma a bomba
    intervalo = -1;
    #define NOTE_C7  2093
    #define NOTE_E7  2637
    #define NOTE_G7  3136
    int notas[] = {NOTE_C7, NOTE_E7, NOTE_G7};
    for (int i = 0; i < 3; i++) {
      tone(0, notas[i]);
      delay(160);
      noTone(0, 0);
    }
  }  
}
inline void mostraDigito(int digito){
  for(int seg=0; seg<7; seg++){
    digitalWrite(portas[seg], digitos[digito][seg]);
  }
}