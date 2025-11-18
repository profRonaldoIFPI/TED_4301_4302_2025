#include <Arduino.h> //necessário apenas no PlatformIO
#include <Servo.h>

#define BUZZER  6
#define LED     5
#define SERVO  A8 // ARDUINO MEGA

Servo servo;

//portas ligadas as segmento A, B, C, D, E, F, G
const int porta[7] =        {7, 8, 9,10,11,12,13};

/* Representação dos segmentos
         A
      F     B
         G
      E     C
         D
*/
int digitos[11][7] = 
  {//A,B,C,D,E,F,G
    {1,1,1,1,1,1,0}, //0 abcdef
    {0,1,1,0,0,0,0}, //1 bc
    {1,1,0,1,1,0,1}, //2 abdeg
    {1,1,1,1,0,0,1}, //3 abcdg
    {0,1,1,0,0,1,1}, //4 bcfg
    {1,0,1,1,0,1,1}, //5 acdfg
    {1,0,1,1,1,1,1}, //6 acdefg
    {1,1,1,0,0,0,0}, //7 abc
    {1,1,1,1,1,1,1}, //8 abcdefg
    {1,1,1,1,0,1,1}, //9 abcdfg
    {0,0,1,1,1,1,1}  //b bcdfg
  }; 

// Array para randomizar a ordem dos pinos
// int pinosAleatorios[5] = {A0, A1, A2, A3, A4, A5}; // ARDUINO UNO
int pinosAleatorios[6] = {42, 44, 46, 48, 50, 52};  // ARDUINO MEGA

void digito(int digito);
void embaralharPinos();  //NOVO
void animacaoDisplay();  //NOVO
void animacaoCobrinha(); //NOVO
void apagarTodos();      //NOVO
void acenderSegmentos(int segmentos[], int quantidade); //NOVO

void setup() {
  for(int i=0; i<7;i++){
    pinMode(porta[i], OUTPUT); //PORTAS DO DISPLAY DE 7 SEGMENTOS
  }
    
  for(int i=42; i<=52; i+=2){ //PORTAS 42 A 52(PARES) PARA DESARMAR A BOMBA
    pinMode(i, INPUT_PULLUP); //HIGH
  }
  
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  servo.attach(SERVO);
  servo.write(0); // angulo 0°
  // Inicializa o gerador de números aleatórios com milis
  randomSeed(millis());
  // Embaralha a ordem dos pinos
  embaralharPinos();
  // Executa animações iniciais
  animacaoDisplay();
  delay(500);
  animacaoCobrinha();
}

int tempoAtual, tempoAnterior = 0, intervalo = 1500, i = 9;
bool buz = false, led = false, reiniciouContador = false;

void loop() {
  tempoAtual = millis();
  if(tempoAtual - tempoAnterior >= intervalo){
    tempoAnterior = tempoAtual;
    digito(i);
    // i = i>9 ? 0 : i; //operador ternário
    if(buz) tone(BUZZER,500,300);
    if(led) digitalWrite(LED, !digitalRead(LED));
    if(i==0){
      tone(BUZZER,1000,5000);
      servo.write(180); //estoura o balão
      digito(10); //b
      while (true){}
    }
    i--;
  }
  /* 
    Portas definidas como INPUT_PULLUP e estão conectado ao GND, por isso o estado é invertido LOW enquanto o fio esta conectado ao GND e HIGH quando desconectado, assim o digitalRead retorna 0 (falso) quando o fio esta conectado ao GND e 1 (verdadeiro) quando desconectado.
  */
  if(digitalRead(pinosAleatorios[0])&&(intervalo!=intervalo/2)){ 
    intervalo = intervalo/2; //acelera o relogio
  }
  if((digitalRead(pinosAleatorios[1]))&&(!buz)){
    buz = true; //buzzer com o relogio
  }
  if(digitalRead(pinosAleatorios[2])){ 
    i = 0; //condição pra explodir 
  }
  if((digitalRead(pinosAleatorios[3]))&&(!led)){
    led = true; //blink com o relogio
  }
  if((digitalRead(pinosAleatorios[4])&&(!reiniciouContador))){
    i = 9; //reinicia o tempo
    reiniciouContador = true;
  }
  if(digitalRead(pinosAleatorios[5])){
    intervalo = -1; //desarma a bomba
    while(true){
      animacaoCobrinha();
    }
  }

}

void digito(int digito){
  for (int segmento = 0; segmento<7 ; segmento++){
    digitalWrite(porta[segmento], digitos[digito][segmento]);
  }
}

// Função para embaralhar o array de pinos usando algoritmo Fisher-Yates
void embaralharPinos() {
  for (int i = 5; i > 0; i--) {
    int j = random(0, i + 1); //  J=2 I=5
    // Troca os elementos
    int temp = pinosAleatorios[i];
    pinosAleatorios[i] = pinosAleatorios[j];
    pinosAleatorios[j] = temp;
  }
}

// Função para apagar todos os segmentos
void apagarTodos() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(porta[i], HIGH); // HIGH apaga o segmento
  }
}

// Função para acender segmentos específicos
void acenderSegmentos(int segmentos[], int quantidade) {
  apagarTodos();
  for (int i = 0; i < quantidade; i++) {
    digitalWrite(porta[segmentos[i]], LOW); // LOW acende o segmento
  }
}

// Função principal de animação
void animacaoDisplay() {
  int delayAnimacao = 50; // Delay entre cada etapa da animação
  
  // Repete a animação N vezes
  #define N 1
  for (int ciclo = 0; ciclo < 3; ciclo++) {
    
    // 1ª Animação: Sentido horário A→B→C→D→E→F
    apagarTodos();
    delay(delayAnimacao);
    
    // Acendendo no sentido horário
    int segmentosHorario1[] = {0, 1, 2, 3, 4, 5}; // A, B, C, D, E, F
    for (int etapa = 0; etapa < 6; etapa++) {
      digitalWrite(porta[segmentosHorario1[etapa]], LOW); // Acende o segmento
      delay(delayAnimacao);
    }
    
    delay(delayAnimacao); // Pausa com todos acesos
    
    // Apagando na mesma ordem horária
    for (int etapa = 0; etapa < 6; etapa++) {
      digitalWrite(porta[segmentosHorario1[etapa]], HIGH); // Apaga o segmento
      delay(delayAnimacao);
    }
    
    delay(delayAnimacao);
    
    // 2ª Animação: Sentido horário A→B→G→F
    apagarTodos();
    delay(delayAnimacao);
    
    int segmentosHorario2[] = {0, 1, 6, 5}; // A, B, G, F
    // Acendendo no sentido horário
    for (int etapa = 0; etapa < 4; etapa++) {
      digitalWrite(porta[segmentosHorario2[etapa]], LOW); // Acende o segmento
      delay(delayAnimacao);
    }
    
    delay(delayAnimacao); // Pausa com todos acesos
    
    // Apagando na mesma ordem horária
    for (int etapa = 0; etapa < 4; etapa++) {
      digitalWrite(porta[segmentosHorario2[etapa]], HIGH); // Apaga o segmento
      delay(delayAnimacao);
    }
    
    delay(delayAnimacao);
    
    // 3ª Animação: Sentido horário C→D→E→G
    apagarTodos();
    delay(delayAnimacao);
    
    int segmentosHorario3[] = {2, 3, 4, 6}; // C, D, E, G
    // Acendendo no sentido horário
    for (int etapa = 0; etapa < 4; etapa++) {
      digitalWrite(porta[segmentosHorario3[etapa]], LOW); // Acende o segmento
      delay(delayAnimacao);
    }
    
    delay(delayAnimacao); // Pausa com todos acesos
    
    // Apagando na mesma ordem horária
    for (int etapa = 0; etapa < 4; etapa++) {
      digitalWrite(porta[segmentosHorario3[etapa]], HIGH); // Apaga o segmento
      delay(delayAnimacao);
    }
    
    // Pausa entre ciclos
    apagarTodos();
    delay(delayAnimacao * 2);
  }
  
  // Finaliza com todos apagados
  apagarTodos();
}

// Função de animação cobrinha desenhando o 8
void animacaoCobrinha() {
  int delayCobrinha = 100; // Delay inicial para movimento da cobrinha
  
  // Sequência para desenhar o 8: A→B→G→E→D→C→G→F
  int sequencia8[] = {0, 1, 6, 4, 3, 2, 6, 5}; // A, B, G, E, D, C, G, F
  
  // Sons do Mario Kart - frequências para tum, tum, biiiim
  int somTum = 400;    // Som grave para "tum"
  int somBiim = 800;   // Som agudo para "biiiim"
  int duracaoTum = 150;
  int duracaoBiim = 300;
  
  // Repete a animação da cobrinha 3 vezes na velocidade normal
  for (int ciclo = 0; ciclo < 3; ciclo++) {
    for (int pos = 0; pos < 8; pos++) {
      apagarTodos();
      digitalWrite(porta[sequencia8[pos]], LOW); // Acende apenas um segmento
      
      // Efeito sonoro sincronizado
      if (pos == 0 || pos == 2) {
        // "Tum" nos segmentos A e G (início de cada volta do 8)
        tone(BUZZER, somTum, duracaoTum);
      } else if (pos == 7) {
        // "Biiiim" no segmento F (final do ciclo)
        tone(BUZZER, somBiim, duracaoBiim);
      }
      
      delay(delayCobrinha);
    }
    // Pequena pausa entre ciclos
    apagarTodos();
    delay(delayCobrinha);
  }
  
  // Agora acelera progressivamente - 5 ciclos acelerando
  for (int cicloAceleracao = 0; cicloAceleracao < 5; cicloAceleracao++) {
    // Reduz o delay a cada ciclo (acelera)
    int velocidadeAtual = delayCobrinha - (cicloAceleracao * 30);
    if (velocidadeAtual < 50) velocidadeAtual = 50; // Velocidade mínima
    
    // Sons também aceleram
    int duracaoTumAtual = duracaoTum - (cicloAceleracao * 20);
    int duracaoBiimAtual = duracaoBiim - (cicloAceleracao * 40);
    if (duracaoTumAtual < 50) duracaoTumAtual = 50;
    if (duracaoBiimAtual < 100) duracaoBiimAtual = 100;
    
    for (int pos = 0; pos < 8; pos++) {
      apagarTodos();
      digitalWrite(porta[sequencia8[pos]], LOW); // Acende apenas um segmento
      
      // Efeito sonoro acelerado
      if (pos == 0 || pos == 2) {
        // "Tum" mais rápido
        tone(BUZZER, somTum + (cicloAceleracao * 50), duracaoTumAtual);
      } else if (pos == 7) {
        // "Biiiim" mais agudo e rápido
        tone(BUZZER, somBiim + (cicloAceleracao * 100), duracaoBiimAtual);
      }
      
      delay(velocidadeAtual);
    }
  }
  
  // Ciclo final super rápido - repete 3 vezes com som contínuo
  for (int cicloFinal = 0; cicloFinal < 3; cicloFinal++) {
    // Som contínuo crescente para o final
    tone(BUZZER, 1000 + (cicloFinal * 200), 240);
    
    for (int pos = 0; pos < 8; pos++) {
      apagarTodos();
      digitalWrite(porta[sequencia8[pos]], LOW);
      delay(30); // Muito rápido
    }
  }
  
  // Som final de "chegada"
  tone(BUZZER, 1500, 500);
  delay(500);
  
  // Finaliza com todos apagados
  apagarTodos();
}