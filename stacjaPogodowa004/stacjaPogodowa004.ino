#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"
#include "charakters.h"

//zmienne
//---------------------------------------------
//LCD
//#define numCols 16 //liczba kolumn
//#define numRows 2 //liczba wierszy
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);//LCD
//przygotowanie tablicy bitów dla znaku specjalnego - stopen

//---------------------------------------------
//Termometr DS18B20
OneWire oneWire(8); //Podłączenie do A5
DallasTemperature tempSensor(&oneWire); //Przekazania informacji do biblioteki
float temp=0.0;
//#define TEMPERATURE_PRECISION 12
//---------------------------------------------
//fotorezystor
#define photoIn  A0//pin analogowy
float photoVal = 0.0;//zmienna - wartosc fotorezystora
//---------------------------------------------
//zegar czsu rzeczywistego(RTC)
DS1307 rtc;
//---------------------------------------------
unsigned long time_1=0;// 
unsigned long time_2=0;// >- zmiana stnu na wyswietalczu 
unsigned long time_3=0;// 
bool state=0;
//---------------------------------------------

void setup() {
  //Serial.begin(9600);
  lcd.createChar(0, customChar);//utworzenie znaku specjalnego z przygotowanej wcześniej tablicy
  lcd.begin(16, 2);
  
  tempSensor.begin();//termometr

  //RTC
  Wire.begin();
  rtc.begin();

  //strart
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  delay(5000);
}

void loop() {
  time_1 = millis();//pobranie czasu mikrokontrolra

  if(time_1-time_2 >= 5000)
  {
    state=!state;
    time_2 = time_1;
  }
  if(time_1-time_3 >= 1000)
  {
    if(state==0)
    {    
      tempSensor.requestTemperatures(); //Pobranie temperatury czujnika
      temp = tempSensor.getTempCByIndex(0);
      photoVal = analogRead(photoIn);//pobranie wartosci z fotorezystora
      //Serial.println(photoVal);
      photoVal = photoVal*5/1024;
      //Serial.println(photoVal);
      //photoVal=photoVal/0.00025;
      photoVal = pow(photoVal, 4)*10;
      
      //Serial.println(photoVal);
      //Serial.println("");
      
      lcd.clear();
      lcd.setCursor(0, 0);//(kolumna, wiersz)
      lcd.print("Temp:");
      lcd.setCursor(6, 0);//(kolumna, wiersz)
      lcd.print(temp);
      lcd.write(byte(0));//wyswietlenie utworzonego znaku specjalnego na LCD
      lcd.print("C");
      lcd.setCursor(0, 1);//(kolumna, wiersz)
      lcd.print("Nasl:");
      lcd.setCursor(6, 1);//(kolumna, wiersz)
      lcd.print(photoVal);
      //lcd.setCursor(6, 10);//(kolumna, wiersz)
      lcd.print(" lx");
    }
    if(state==1)
    {
      DateTime now = rtc.now();
  
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(toDigits(now.day()));
      lcd.print('/');
      lcd.print(toDigits(now.month()));
      lcd.print('/');
      lcd.print(toDigits(now.year()));
      lcd.setCursor(0, 1);
      lcd.print(toDigits(now.hour()));
      lcd.print(':');
      lcd.print(toDigits(now.minute()));
      lcd.print(':');
      lcd.print(toDigits(now.second()));
    }
    time_3 = time_1;
  }
}

String toDigits(int number)
{
  String text="";

  if(number>=0 && number<10)
  {
    text="0"+String(number);
  }
  else
  {
    text=String(number);
  }

  return text;
}
