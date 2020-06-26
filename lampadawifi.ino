////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arquivo:   NodeMCU_Lamp.ino
//Tipo:      Codigo-fonte para utilizar no NodeMCU atraves da IDE do Arduino
//Autor:     Marco Rabelo para o canal Infortronica Para Zumbis (www.youtube.com/c/InfortronicaParaZumbis)
//Descricao: Curso de ESP8266 - Controlando uma lampada do celular, PC, etc.
//Video:     https://youtu.be/t103WbEr2Kc
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>

//Nome da sua rede Wifi
const char* ssid = "[ssid_redewifi]";

//Senha da rede
const char* password = "[senha_wifi]";

//IP do ESP (para voce acessar pelo browser)
IPAddress ip(192, 168, 1, 8);

//IP do roteador da sua rede wifi
IPAddress gateway(192, 168, 1, 1);

//Mascara de rede da sua rede wifi
IPAddress subnet(255, 255, 255, 0);

//Criando o servidor web na porta 80
WiFiServer server(80);

//Pino do NodeMCU que estara conectado ao rele
const int pin = 4; //Equivalente ao D2 no NodeMCU

//Funcao que sera executada apenas ao ligar o ESP8266
void setup() {
  //Preparando o pino, que esta lidago ao rele
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);

  //Conectando a rede Wifi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  
  
  //Verificando se esta conectado,
  //caso contrario, espera um pouco e verifica de novo.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Iniciando o servidor Web
  server.begin();
}

//Funcao que sera executada indefinidamente enquanto o NodeMCU estiver ligado.
void loop() {
  //Verificando se o servidor esta pronto.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Verificando se o servidor recebeu alguma requisicao
  while (!client.available()) {
    delay(1);
  }

  //Obtendo a requisicao vinda do browser
  String req = client.readStringUntil('\r');
  
  //Sugestao dada por Enrico Orlando
  if(req == "GET /favicon.ico HTTP/1.1"){
      req = client.readStringUntil('\r');
  }
  
  client.flush();

  //Iniciando o buffer que ira conter a pagina HTML que sera enviada para o browser.
  String buf = "";

  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<head> ";
  buf += "<meta charset='UTF-8'> ";
  buf += "<meta http-equiv='cache-control' content='max-age=0' /> ";
  buf += "<meta http-equiv='cache-control' content='no-cache' /> ";
  buf += "<meta http-equiv='expires' content='0' /> ";
  buf += "<meta http-equiv='expires' content='Tue, 01 Jan 1980 1:00:00 GMT' /> ";
  buf += "<meta http-equiv='pragma' content='no-cache' /> ";
  buf += "<title>Automa&ccedil;&atilde;o Residencial</title> ";
  buf += "<style> ";
  buf += "body{font-family:Open Sans; color:#555555;} ";
  buf += "h1{font-size:24px; font-weight:normal; margin:0.4em 0;} ";
  buf += ".container { width: 100%; margin: 0 auto; } ";
  buf += ".container .row { float: left; clear: both; width: 100%; } ";
  buf += ".container .col { float: left; margin: 0 0 1.2em; padding-right: 1.2em; padding-left: 1.2em; } ";
  buf += ".container .col.four, .container .col.twelve { width: 100%; } ";
  buf += "@media screen and (min-width: 767px) { ";
  buf += ".container{width: 100%; max-width: 1080px; margin: 0 auto;} ";
  buf += ".container .row{width:100%; float:left; clear:both;} ";
  buf += ".container .col{float: left; margin: 0 0 1em; padding-right: .5em; padding-left: .5em;} ";
  buf += ".container .col.four { width: 50%; } ";
  buf += ".container .col.tweleve { width: 100%; } ";
  buf += "} ";
  buf += "* {-moz-box-sizing: border-box; -webkit-box-sizing: border-box; box-sizing: border-box;} ";
  buf += "a{text-decoration:none;} ";
  buf += ".btn {font-size: 18px; white-space:nowrap; width:100%; padding:.8em 1.5em; font-family: Open Sans, Helvetica,Arial,sans-serif; ";
  buf += "line-height:18px; display: inline-block;zoom: 1; color: #fff; text-align: center; position:relative; ";
  buf += "-webkit-transition: border .25s linear, color .25s linear, background-color .25s linear; ";
  buf += "transition: border .25s linear, color .25s linear, background-color .25s linear;} ";
  buf += ".btn.btn-sea{background-color: #08bc9a; border-color: #08bc9a; -webkit-box-shadow: 0 3px 0 #088d74; box-shadow: 0 3px 0 #088d74;} ";
  buf += ".btn.btn-sea:hover{background-color:#01a183;} ";
  buf += ".btn.btn-sea:active{ top: 3px; outline: none; -webkit-box-shadow: none; box-shadow: none;} ";
  buf += "</style> ";
  buf += "</head> ";
  buf += "<body> ";
  buf += "<div class='container'> ";
  buf += "<div class='row'> ";
  buf += "<div class='col twelve'> ";
  buf += "<p align='center'><font size='10'>Controle de l&acirc;mpadas</font></p> ";
  buf += "</div> ";
  buf += "</div> ";
  buf += "<div class='row'> ";
  buf += "<div class='col four'> ";
  buf += "<a href='?f=on' class='btn btn-sea'>Ligar</a> ";
  buf += "</div> ";
  buf += "<div class='col four'> ";
  buf += "<a href='?f=off' class='btn btn-sea'>Desligar</a> ";
  buf += "</div> ";
  buf += "</div> ";
  buf += "<div class='col twelve'> ";
  buf += "<p align='center'><font size='5'>Infortr&ocirc;nica Para Zumbis</font></p> ";
  buf += "</div> ";
  buf += "</div> ";
  buf += "</body> ";
  buf += "</html> ";

  //Enviando para o browser a 'pagina' criada.
  client.print(buf);
  client.flush();

  //Analisando a requisicao recebida para decidir se liga ou desliga a lampada
  if (req.indexOf("on") != -1)
  {
    digitalWrite(pin, LOW);
  }
  else if (req.indexOf("off") != -1)
  {
    digitalWrite(pin, HIGH);
  }
  else
  {
    //Requisicao invalida!
    client.stop();
  }
}

