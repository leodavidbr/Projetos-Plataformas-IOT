#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define PIN_RED 11
#define PIN_GREEN 12
#define PIN_BUZZER 13

#define PINO_SS 9
#define PINO_RESET 8

MFRC522 mfrc522(PINO_SS, PINO_RESET);
int qntErros = 0;

String readRfid();

void acessoGarantido() {
  qntErros = 0;

  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GREEN, HIGH);
  Serial.println("Acesso liberado");

  delay(10000);

  digitalWrite(PIN_GREEN, LOW);
}

void piscarLed(int led, int times, int tLigado, int tDesligado) {
  int estadoInicial = digitalRead(led);
  for(int i = 0; i < times; i++) {
    digitalWrite(led, LOW);
    delay(tDesligado);
    digitalWrite(led, HIGH);
    delay(tLigado);
  }
  digitalWrite(led, estadoInicial);
}

void perigo() {
  Serial.println("SISTEMA BLOQUEADO");

  digitalWrite(PIN_BUZZER, HIGH);
  for(int i = 0; i < 2; i++) {
    digitalWrite(PIN_RED, LOW);
    delay(250);
    digitalWrite(PIN_RED, HIGH);
    delay(250);
  }
  digitalWrite(PIN_BUZZER, LOW);
}

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_RED, HIGH);
  Serial.println("Sistema Inicializado: Aproxime o Token");
}

void loop()
{
  String conteudoLido = readRfid();
  if (conteudoLido == "")
  {
    return;
  }
  
  if(conteudoLido == "99 01 3D 53" ) {
    acessoGarantido();
  } else {
    if(qntErros >= 5) {
      perigo();
      qntErros = 0;
    } else {
      qntErros++;

      Serial.println("Cartão inválido");
      piscarLed(PIN_RED, 3, 250, 250);
    }
  }
  digitalWrite(PIN_RED, HIGH);
}

String readRfid()
{
  if (!mfrc522.PICC_IsNewCardPresent()){
    return "";
  }
  if (!mfrc522.PICC_ReadCardSerial()){
    return "";
  }  

  Serial.print("UID da tag:"); 

  String conteudo = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    // Serial.print(mfrc522.uid.uidByte[i], HEX);

    if (mfrc522.uid.uidByte[i] < 0x10)
    {
      conteudo.concat(String(" 0"));
    }
    else
    {
      conteudo.concat(String(" "));
    }
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  // Serial.println();
  conteudo.toUpperCase();
  
  return conteudo.substring(1);
}