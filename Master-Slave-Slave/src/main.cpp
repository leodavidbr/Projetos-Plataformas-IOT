#include <Arduino.h>
#include <SoftwareSerial.h>

#define LED_PIN 7

SoftwareSerial master(10, 11);

String msg = "";
int sensorVal = 0, oldSensorVal = 0, botaoVal = 0, oldBotaoVal = 0;
int ledVal = 0, oldLedVal = 0;

void readMasterPort();
void convertToMsg();

void setup() {
  Serial.begin(9600);
  master.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  readMasterPort();
  convertToMsg();

  analogWrite(LED_PIN, botaoVal ? 255 : sensorVal);

  if(sensorVal < 20) {
    Serial.println("Baixa luminosidade!");
  }

  delay(200);
}

void readMasterPort() {
  msg = "";
  while(master.available()) {
    delay(10);
    //if(master.available() > 0) {
      char c = master.read();
      msg += c;
      //Serial.print("mensagem: ");
      //Serial.println(msg);
    //}
  }
  master.flush();
}

void convertToMsg() {
  if(msg.length() > 0) {

    int index = msg.indexOf(";");
    String botaoStr = msg.substring(0, index);
    String sensorStr = msg.substring(index+1, msg.length());

    char carray1[6];
    botaoStr.toCharArray(carray1, sizeof(carray1));
    botaoVal = atoi(carray1);  

    char carray2[6];
    sensorStr.toCharArray(carray2, sizeof(carray2));
    sensorVal = atoi(carray2);

    sensorVal = map(sensorVal, 0, 1023, 0, 255);  

    Serial.print("Sensor: ");
    Serial.println(sensorVal);

    Serial.print("Botao: ");
    Serial.println(botaoVal);
  }  
}
