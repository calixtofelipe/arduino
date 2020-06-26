#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

const char* ssid = "[ssid_wifi]";
const char* password = "[senha_wifi]";

unsigned long previousMillis = 0;
const long interval = 300000;  //de 5 em 5 minutos

unsigned long previousMillisDois = 0;
const long intervalDois = 7200000;  //de 4 em 4 horas

//Pino do NodeMCU que estara conectado ao rele
//const int pin0 = 16; //Equivalente ao D0 no NodeMCU
const int pin1 = 5;  //Equivalente ao D1 no NodeMCU
const int pin2 = 4;  //Equivalente ao D2 no NodeMCU
//const int pin3 = 0;  //Equivalente ao D3 no NodeMCU
//const int pin4 = 2;  //Equivalente ao D4 no NodeMCU
const int pin5 = 14; //Equivalente ao D5 no NodeMCU
const int pin6 = 12; //Equivalente ao D6 no NodeMCU
//const int pin7 = 13; //Equivalente ao D7 no NodeMCU
ESP8266WebServer server(5874);

const char* www_username = "[user_name]";
const char* www_password = "[password]";
//IP do ESP (para voce acessar pelo browser)
IPAddress ip(192, 168, 0, 25);

//IP do roteador da sua rede wifi
IPAddress gateway(192, 168, 0, 1);

//Mascara de rede da sua rede wifi
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);//
  //pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  //pinMode(pin3, OUTPUT);
  //pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  //pinMode(pin7, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();

  server.on("/", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    } else {
      //digitalWrite(pin3, HIGH);
      Serial.print(server.argName(0) + "arg:  " + server.arg(0) );
      if (server.arg(0).equals("LIGARGERAL")) {
        //digitalWrite(pin0, LOW);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        //digitalWrite(pin3, LOW);
        //digitalWrite(pin4, LOW);
        digitalWrite(pin5, LOW);
        digitalWrite(pin6, LOW);
        //digitalWrite(pin7, LOW);
        Serial.println("ENTROU NO LIGARGERAL" );
      }
      if (server.arg(0).equals("DESLIGARGERAL")) {
        //digitalWrite(pin0, HIGH);
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
        //digitalWrite(pin3, HIGH);
        //digitalWrite(pin4, HIGH);
        digitalWrite(pin5, HIGH);
        digitalWrite(pin6, HIGH);
        //digitalWrite(pin7, HIGH);
        Serial.println("ENTROU NO DESLIGARGERAL" );
      }
      if (server.arg(0).equals("LIGARRELE01")) {
        digitalWrite(pin1, LOW);
      }
      if (server.arg(0).equals("DESLIGARRELE01")) {
        digitalWrite(pin1, HIGH);
      }
      if (server.arg(0).equals("LIGARRELE02")) {
        digitalWrite(pin2, LOW);
      }
      if (server.arg(0).equals("DESLIGARRELE02")) {
        digitalWrite(pin2, HIGH);
      }
      if (server.arg(0).equals("LIGARRELE03")) {
        digitalWrite(pin5, LOW);
      }
      if (server.arg(0).equals("DESLIGARRELE03")) {
        digitalWrite(pin5, HIGH);
      }
      if (server.arg(0).equals("LIGARRELE04")) {
        digitalWrite(pin6, LOW);
      }
      if (server.arg(0).equals("DESLIGARRELE04")) {
        digitalWrite(pin6, HIGH);
      }
      if (server.arg(0).equals("DESLIGARWIFI")) {
        WiFi.disconnect();
        Serial.println(WiFi.macAddress());
        Serial.println("WIFI-STATUS" + WiFi.status());
        pinMode(pin1, INPUT);
        pinMode(pin2, INPUT);
        pinMode(pin5, INPUT);
        pinMode(pin6, INPUT);
      }
      server.send(200, "text/html", "retorno ok");
    }
  });
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
  Serial.println(WiFi.macAddress());
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  unsigned long currentMillis = millis();
  unsigned long currentMillisDois = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   // WiFi.waitForConnectResult() WiFi.status() != WL_CONNECTED
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi Connect Failed! Rebooting...");
      WiFi.mode(WIFI_STA);
      WiFi.config(ip, gateway, subnet);
      WiFi.begin(ssid, password);
      delay(10000);
      if (WiFi.status() != WL_CONNECTED) {
        ESP.restart();
      }
    }
  }
  if (currentMillisDois - previousMillisDois >= intervalDois) {
    previousMillisDois = currentMillisDois;
    Serial.println("reinicia de 2 em duas horas");
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid, password);
    delay(10000);
  }
}


