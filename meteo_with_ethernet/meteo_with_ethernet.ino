//..................................................................
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <SPI.h>
#include <Ethernet2.h>
//.............................ИМПОРТИРУЕМ ВСЕ НЕОБХОДИМЫЕ БИБЛИОТЕКИ
//...................................................................
#define DHTPIN 4
#define DHTTYPE DHT11 
#define MOTOR_PIN  9
int ss,modes,flag,pot,o,svet,p;
String inf_svet, inf_cool;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
//..............................ИНИЦИАЛИЗИРУЕМ ВСЕ НУЖНЫЕ ПЕРЕМЕННЫЕ
//..................................................................
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2); 
IPAddress ip(192, 168, 48, 20);
EthernetServer server(80);
//..............................ОБЪЯВЛЯЕМ ВСЕ ИСПОЛЬЗУЕМЫЕ УСТРОЙСТВА
//...................................................................
int tempC(float a)
{
  lcd.print("Temp:");
  lcd.print(a);
  lcd.print("C");
}
int hum(float a)
{
  lcd.print("Humidity:");
  lcd.print(a);
  lcd.print("%");
}
//................................СОЗДАЕМ ФУНКЦИИ ДЛЯ ВЫВОДА ТЕМПЕРАТУРЫ И ВЛАЖНОСТИ
//................................................................................
void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
//  pinMode(10,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  dht.begin();
  lcd.init();                     
  lcd.backlight();
  Serial.print("Сервер запущен на ip-адресе: ");
  Serial.println(Ethernet.localIP());
}
//..............................НАСТРАИВАЕМ ВСЕ НЕОБХОДИМЫЕ ПИНЫ(НА ВХОД ИЛИ ВЫХОД)

void loop()
{
  EthernetClient klient = server.available();
  //..........ЧАСТЬ КОДА,КОТОРАЯ ПОСТОЯННО ОБНОВЛЯЕТ ДАННЫЕ О ТЕМПЕРАТУРЕ,ВЛАЖНОСТИ И СОСТОЯНИИ КНОПКИ,ОТВЕЧАЮЩЕЙ ЗА ВКЛЮЧЕНИЕ И ВЫКЛЮЧЕНИЕ СИСТЕМЫ ОСВЕЩЕНИЯ
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  ss = digitalRead(7);
  //.........................................................................................................................................................
  if (ss == 1)//..........................ВКЛЮЧЕНИЕ СИСТЕМЫ ОСВЕЩЕНИЯ
  {
    ss = digitalRead(7);
    modes = digitalRead(8);
    if (modes == 0)
    {
      inf_svet = "hand";
      flag = 1;
      digitalWrite(5,LOW);
      digitalWrite(3,HIGH);
    }
    if(modes == 1)
    {
      inf_svet = "automatic";
      flag = 0;
      digitalWrite(5,HIGH);
      digitalWrite(3,LOW);
    }
    delay(10);
      if(flag == 1)
      {
        pot = analogRead(A0);
        o = pot / 4;
        analogWrite(6,o);
      }
      if(flag == 0)
      { 
        svet = analogRead(A1);
        p = (1024 - svet) / 0.8;
        analogWrite(6,p);
  }
  }
  //..........................................
  else
  //......................ВЫКЛЮЧЕНИЕ СИСТЕМЫ ОСВЕЩЕНИЯ
  {
  digitalWrite(5,LOW);
  digitalWrite(3,LOW);
//  digitalWrite(10,LOW);
  }
  //..................................................







  
  if (t > 30)
  //..................ВКЛЮЧЕНИЕ КУЛЕРА
  {
    inf_cool = "Cooling system works";
    analogWrite(9,255);
  }
  //...................................
  //...............ВЫКЛЮЧЕНИЕ КУЛЕРА
  else
  {
    inf_cool = "Cooling system off";
    analogWrite(9,0);
  }
  //..................................
  //...........................ПОСТОЯННЫЙ ВЫВОД ТЕМПЕРАТУРЫ И ВЛАЖНОСТИ НА ЭКРНА
  if (klient) {
    boolean flag_ethernet = true;
    while (klient.connected()) {
      if (klient.available()) {
        char c = klient.read();
        Serial.write(c);
        if (c == '\n' && flag_ethernet) {
          klient.println("HTTP/1.1 200 OK");
          klient.println("Content-Type: text/html");
          klient.println("Connection: close");
          klient.println("Refresh: 5");
          klient.println();
          klient.println("");
          klient.println("");
          klient.println("DATA HOUSE: ");
          klient.println("<br />");
          klient.print("Temperature: ");
          klient.println(t);
          klient.println("Humidity: ");
          klient.print(h);
          klient.println("<br />");
          klient.println("<br />");
          klient.print("HOME SYSTEM STATUS:");
          klient.println("<br />");
          klient.print("Lighting system mode: ");
          klient.print(inf_svet);
          klient.println("<br />");
          klient.print("Cooling system status: ");
          klient.print(inf_cool);
          break;
        }
      if (c == '\n') {
          flag_ethernet = true;
        } else if (c != '\r') {
          flag_ethernet = false;
        }
      }
      }

      delay(1);
    klient.stop();
    Serial.println("Клиент подключен.");
    Serial.println("---------------------");
  }
  lcd.setCursor(0,0);
  tempC(t);
  lcd.setCursor(0,1);
  hum(h);
  delay(1000);
  //.............................................................................

}
