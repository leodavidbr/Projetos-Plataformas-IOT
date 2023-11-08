#include <Arduino.h>

// FIXME: definir corretamente as portas
#define LED_INICIO 19
#define LED_JOGADOR_1 18
#define LED_JOGADOR_2 21
#define TOUCH_INICIO_JOGO 12
#define TOUCH_JOGADOR_1 32
#define TOUCH_JOGADOR_2 33

#define TOUCH_SENSIBILIDADE 30

int pontosJogador1 = 0;
int pontosJogador2 = 0;

// -1 -> resetar jogo | 0 -> parado | 1 -> comecando | 2 -> esperando jogador
int estadoJogo = -1;
int momentoInicial = 0;
int momentoClique = 0;
int resultadoJogo = 0;

bool verificarInicio();
void iniciarJogo();
void verificarToqueJogadores();
void piscarLed(int pino, int vezes, int tempoLigado, int tempoDesligado);
void mostrarPlacarAtual();
void resetarJogo();

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_INICIO, OUTPUT);
  pinMode(LED_JOGADOR_1, OUTPUT);
  pinMode(LED_JOGADOR_2, OUTPUT);

  digitalWrite(LED_INICIO, HIGH);
}

void loop() {
  // Serial.println("nova leitura");
  // Serial.println(touchRead(TOUCH_INICIO_JOGO));
  // Serial.println(touchRead(TOUCH_JOGADOR_1));
  // Serial.println(touchRead(TOUCH_JOGADOR_2));
  // delay(1000);
  // return;
  switch (estadoJogo)
  {
  case -1:
    resetarJogo();
     break;
  case 0:
    verificarInicio();
    break;
  case 1:
    iniciarJogo();
    break;
  case 2:
    verificarToqueJogadores();
    break;
  case 3:
    mostrarPlacarAtual();
    break;
  default:
    estadoJogo = -1;
    break;
  }
}

void resetarJogo(){
  momentoInicial = 0;
  momentoClique = 0;
  resultadoJogo = 0;
  digitalWrite(LED_INICIO, HIGH);
  estadoJogo = 0;

  Serial.println("Toque no fio de inicio para iniciar o jogo.");
}

bool verificarInicio(){
  if(touchRead(TOUCH_INICIO_JOGO) <= TOUCH_SENSIBILIDADE){
    estadoJogo = 1;
    return true;
  }
  else {
    estadoJogo = 0;
    return false;
  }
}

void iniciarJogo(){
  Serial.println("Iniciando jogo. Se prepare");
  piscarLed(LED_INICIO, 3, 1000, 400);
  digitalWrite(LED_INICIO, LOW);

  estadoJogo = 2;
  momentoInicial = millis();
}

void verificarToqueJogadores(){
  bool jogador1Tocou = false;
  bool jogador2Tocou = false;
  
  if(touchRead(TOUCH_JOGADOR_1) <= TOUCH_SENSIBILIDADE) {
    jogador1Tocou = true;
    momentoClique = millis();
  }

  if(touchRead(TOUCH_JOGADOR_2) <= TOUCH_SENSIBILIDADE) {
    jogador2Tocou = true;
    momentoClique = millis();
  }

  estadoJogo = 3;

  if(jogador1Tocou && jogador2Tocou) {
    resultadoJogo = 3;
    pontosJogador1++;
    pontosJogador2++;
  } else if(jogador1Tocou) {
    resultadoJogo = 1;
    pontosJogador1++;
  } else if(jogador2Tocou) {
    resultadoJogo = 2;
    pontosJogador2++;
  } else {
    resultadoJogo = 0;
    estadoJogo = 2;
  }
}

void piscarLed(int pino, int vezes, int tempoLigado, int tempoDesligado){
  for(int i = 0; i < vezes; i++) {
    digitalWrite(pino, HIGH);
    Serial.println("pi");
    delay(tempoLigado);
    digitalWrite(pino, LOW);
    delay(tempoDesligado);
  }
}

void mostrarPlacarAtual(){
  Serial.println("--------- *** ---------");
   if((momentoClique - momentoInicial) <= 20) {
    if(resultadoJogo == 3) {
      Serial.println("Ambos os jogadores roubaram!!");
    } else {
      Serial.print("Jogador   ");
      Serial.print(resultadoJogo);
      Serial.println(" roubou!!");
    }

    if(resultadoJogo == 1)
      pontosJogador1--;
    if(resultadoJogo == 2)
      pontosJogador2--;
    if(resultadoJogo == 3){
      pontosJogador1--;
      pontosJogador2--;
    }
    
    estadoJogo = -1;
    return;
  }

  if(resultadoJogo == 3){
    Serial.println("Empatou nessa rodada!!!!!");
  }else{
    Serial.print("O Jogador ");
    Serial.print(resultadoJogo);
    Serial.println(" foi o mais rapido");
  }

  Serial.print("tempo para o clique ");
  Serial.print((momentoClique - momentoInicial) / 1000.0f);
  Serial.println(" segundos");


  Serial.println("Placar:");
  Serial.print("Jogador 1: ");
  Serial.print(pontosJogador1);
  Serial.println(" pontos");

  Serial.print("Jogador 2: ");
  Serial.print(pontosJogador2);
  Serial.println(" pontos");

  Serial.println("--------- *** ---------");

  estadoJogo = -1;
}

