// Practica de Arduino Modulo 2 - OLED DHT22 y Reloj
//importacion de librerias
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>
#include <DHT_U.h>
 //
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 rtc;
//Inicio de varaibles globales e iniciales
void setup() {
  rtc.begin();
  dht.begin();
  lcd.init();

  lcd.backlight();
  lcd.print("Bienvenido");

  delay(2000);
}
//Llamada a metodo de sensores
void loop() {
  sensores();
}

void sensores() {
  //Llamado a datos de los sensores
  DateTime fecha = rtc.now();
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  //hora
  lcd.clear();
  lcd.print("La hora es:");
  lcd.setCursor(0, 1);
  imprimirDigito(fecha.hour());
  lcd.print(":");
  imprimirDigito(fecha.minute());
  lcd.print(":");
  imprimirDigito(fecha.second());
  delay(1500);

  //fecha
  lcd.clear();
  lcd.print("La fecha es:");
  lcd.setCursor(0, 1);
  imprimirDigito(fecha.day());
  lcd.print("/");
  imprimirDigito(fecha.month());
  lcd.print("/");
  imprimirDigito(fecha.year());
  delay(1500);

  //temperatura
  lcd.clear();
  lcd.print("La temperatura es:");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(temperatura);
  lcd.print("C");
  delay(1500);

  //humedad
  lcd.clear();
  lcd.print("La humedad es:");
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(humedad);
  lcd.print("%");
  delay(1500);


}
//Metodo de impresion de numero
void imprimirDigito(int num) {
  if (num < 10) {
    lcd.print("0");
  }
  lcd.print(num);
}
