#include <Arduino.h>
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

void mostraDigito(int digito);

void setup() {
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
int digito = 0, tempoAtual, tempoAnterior = 0, intervalo = 1000;

void loop() {
  tempoAtual = millis(); //tempo que o arduino está ligado
  if(tempoAtual - tempoAnterior >= intervalo){ //entra aqui a cada intervalo 
    tempoAnterior = tempoAtual;
    mostraDigito(digito);
    digito++;
    digito = digito>9 ? 0 : digito;
  }


  // randomSeed(millis());
  // digito = random(0,9); //entre 0 e 9
  
}
inline void mostraDigito(int digito){
  for(int seg=0; seg<7; seg++){
    digitalWrite(portas[seg], digitos[digito][seg]);
  }
}