#include <Arduino.h>
#include <SoftwareSerial.h>

#define SENSOR_PIN A2 // por padrao é input
#define BOTAO_PIN 7

SoftwareSerial slave(10, 11);

String msg;
int sensorVal = 0, botaoVal = 0;
int oldSensorVal = 0;
int oldBotaoVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  slave.begin(9600);  
  pinMode(BOTAO_PIN, INPUT);
  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorVal = analogRead(SENSOR_PIN);
  botaoVal = digitalRead(BOTAO_PIN);
  Serial.print("Sensor: ");
  Serial.println(sensorVal);
  Serial.print("Botao: ");
  Serial.println(botaoVal);
  
  oldSensorVal = sensorVal;
  oldBotaoVal = botaoVal;
 
  String msgToSend = String(botaoVal) + ";" + String(sensorVal);
  slave.print(msgToSend);
  Serial.print("Mestre enviou ");
  Serial.println(msgToSend);

  delay(150);
}