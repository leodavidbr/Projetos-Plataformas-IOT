#include <WiFi.h>
#include <SPIFFS.h>
#include "time.h"
#include <String.h>

const char* ssid       = "paulo";
const char* password   = "paulolegal";
const char* fileName = "/logsLed.txt";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

int ledState = 0;
int botState = 0;

String s;

String printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "Failed";
  }
  
  String ans = "";
  
  ans += String(timeinfo.tm_wday) + "/";
  ans += String(timeinfo.tm_mon) + "/";
  ans += String(timeinfo.tm_year) + " ";

  ans += String(timeinfo.tm_hour) + ":";
  ans += String(timeinfo.tm_min) + ":";
  ans += String(timeinfo.tm_sec) + ";";

  return ans;
}

void writeFile(String state, String path) { //escreve conteúdo em um arquivo
  File rFile = SPIFFS.open(path, "a");//a para anexar
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    Serial.print("tamanho ");
    Serial.println(rFile.size());
    rFile.println(state);
    Serial.print("Gravou: ");
    Serial.println(state);
  }
  rFile.close();
}

String readFile(String path) {
  Serial.println("Read file");
  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    Serial.print("----------Lendo arquivo ");
    Serial.print(path);
    Serial.println("  ---------");
    while (rFile.position() < rFile.size())
    {
      s = rFile.readStringUntil('\n');
      s.trim();
      Serial.println(s);
    }
    rFile.close();
    return s;
  }

  return "";
}

int getLedState(String readStr) {
  int splitChar = readStr.indexOf(";");
  String hora = readStr.substring(0, splitChar);

  String stateStr = readStr.substring(splitChar, readStr.length());

  return stateStr.toInt();
}

void formatFile() {
  Serial.println("Formantando SPIFFS");
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}

void openFS(void) {
  if (!SPIFFS.begin()) {
    Serial.println("\nErro ao abrir o sistema de arquivos");
  }
  else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}

void checkBot(){
  int prevLedState = ledState;
  int prevBotState = botState;
  
  botState = digitalRead(19);

  if(prevBotState == 0 && botState == 1) {
    ledState = !ledState;
    digitalWrite(18, ledState);
  
    String ans = "";
    ans = printLocalTime() + String(ledState) + "\n";
    Serial.println(ans);
    delay(500);
  }

  //writeFile()
  //Serial.println(ledState);
}

void setup() {
  Serial.begin(115200);
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //printLocalTime();

  delay(500);

  formatFile();
  openFS();

  String readStr = readFile(fileName);

  Serial.print("li ");
  Serial.println(readStr);

  if(readStr == "") {
    ledState = 0;
  } else {
    ledState = getLedState(readStr);
  }

  Serial.print("o estado do led eh ");
  Serial.println(ledState);
  //disconnect WiFi as it's no longer needed
  // WiFi.disconnect(true);
  // WiFi.mode(WIFI_OFF);
  pinMode(18, OUTPUT);
  pinMode(19, INPUT);

  digitalWrite(18, ledState);
}

void loop()
{
  checkBot();
}
