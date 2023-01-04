#include <BluetoothSerial.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <gamma.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//tirar a nescessidade do WIFI
//
BluetoothSerial ESP_BT;

const char* ssid = "CASA";                  // Nome da rede Wi-Fi
const char* password = "24689632";     // Senha da rede Wi-Fi

float millis1 = 0;//temporizador
byte saida = 0;

//configura o painel RGB
  #define PIN 26 // Pino do display

  Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32,8, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
  int x    = matrix.width();
  int pass = 0;
  int x1 = 0;
//paleta de cores do painel

  uint32_t white = matrix.Color(255, 255, 255);
  uint32_t blue = matrix.Color(0, 0, 255);
  uint32_t red = matrix.Color(255, 0, 0);
  uint32_t green = matrix.Color(0, 255, 0);
  uint32_t orange = matrix.Color(255, 153, 0);
  uint32_t purple = matrix.Color(255, 0, 255);
  uint32_t gold = matrix.Color(153, 153, 0);
  uint32_t black = matrix.Color(0, 0, 0);
  uint32_t selColor, currentColor;
  uint8_t r, g, b;
  uint32_t AliceBlue = 0xF0F8FF;
  uint32_t Chocolate = 0xD2691E;

//captura hora e adiciona na string hora
  WiFiUDP udp;
  NTPClient ntp(udp, "br.pool.ntp.org", -3*3600, 60000);//configurações do servidor NTP-(udp, "servidor", fuso*3600, temp_atualiza)
  String hora;
 
void setup()  {
  //inicia wifi e captura o horario
   Serial.begin(115200);
    WiFi.begin(ssid, password);

   while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    }
   Serial.println("");
   Serial.println("WiFi connected.");
   ntp.begin();              
  //inicia o painel
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(5);
    matrix.setTextColor(corAleatoria ());
  //incia BT
    ESP_BT.begin("RELOGIO_IRMABOTS");
  }

void loop() {
  static String brilho_string = "5";
  static int i = 50;
  static String matrix_string = "mama mia-mia mama";
  int y=(-6*(i));
  char indentificador;
  static char caracter2='0';

  ntp.update();//atualiza o horario
  hora = ntp.getFormattedTime();//descobre o horario atual e a põe em hora
  piscahora(600,1);//exibe hora com efeito de piscar - (tempo de pisca, x pisca)
  
  if(ESP_BT.available()){
    indentificador = ESP_BT.read();
    Serial.print("estou conectado:");
    Serial.println(indentificador);
  }
  switch(indentificador){
    case '1':
    i=0;
    matrix_string = "";
    while(ESP_BT.available()){
      char caracter = ESP_BT.read();
      matrix_string += caracter;
      Serial.println(matrix_string);
      Serial.println(caracter);
      i++;
    }
    indentificador='9';
    break;

    case '2':
    Serial.println("LETS GO");
    brilho_string = "";
    int brilho;
    do{
     while(caracter2!='/'){
       caracter2 = ESP_BT.read();
       brilho_string += caracter2;
       Serial.println(caracter2);
      }
      caracter2 = '0';
      brilho = brilho_string.toInt();
      matrix.setBrightness(brilho);
      brilho_string = "";
    }while(ESP_BT.available());

    Serial.print("BRILHO ATUAL: ");
    Serial.println(brilho);    
    indentificador='91';
    break;
  }  

 float millis2 = millis();//temporizador

  //exibição com efeito de passagem painel
   while(millis2-millis1>1200){
    millis2 = millis();
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    matrix.print(matrix_string);
      if(--x < y) {
      x = matrix.width();
      millis1 = millis2;
      if(++pass >= 3) pass = 0;
        matrix.setTextColor(corAleatoria ());
        }
    matrix.show();
    delay(50);
   }
  }   
//função que pisca a hora
  void piscahora(int x, byte y3){
   byte y2 = 0;
    while(y3>=y2){
      matrix.setCursor(2, 0);
      matrix.print(hora); 
      matrix.show();
     delay(x);
      matrix.fillScreen(0);
      matrix.show();
     delay(x);
     matrix.setTextColor(corAleatoria ());
     y2++;
    }
  }
//função que escolhe uma cor aleatoria para o painel
  uint32_t corAleatoria () {

    int num = random (1, 6);

    switch (num) {
      case 1:
        return Chocolate;
        break;
      case 2:
        return blue;
        break;
      case 3:
        return red;
        break;
      case 4:
        return green;
        break;
      case 5:
        return orange;
        break;
      case 6:
        return purple;
        break;

      default:
        return black;
        break;
    }
  }