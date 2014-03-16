
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <EEPROM.h>
#include "RTClib.h"
#include <math.h>

RTC_Millis rtc;
int sensorPin = A0;
int adc = 0;

int i=0,cookTemp=185;
int cookTime=120;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#define WHITE 0x7

void setup() {
  lcd.begin(16, 2);
  rtc.begin(DateTime(__DATE__, __TIME__));
  int time = millis();
  lcd.print("Hello, world!");
  lcd.setBacklight(WHITE);
  pinMode(13,OUTPUT);
}

void loop() {
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_DOWN) editTemp();
    if (buttons & BUTTON_UP) temperature();
    if (buttons & BUTTON_SELECT){
      editTime();
    }
    //if (buttons & BUTTON_LEFT) 
  }
}

void temperature(){
  i=0;
  int v0 = 0;
  delay(1000);
  uint8_t buttons = lcd.readButtons();
  lcd.print("TEMPERATURE ");
  while(true){
    buttons = lcd.readButtons(); //this is necessary?
    lcd.setCursor(0, 1);
    v0 = analogRead(sensorPin);
    float on = v0*(5/1023.0);
    float rt = 9870.0/((5/on)-1.0);
    float logrt = log(rt);
    float temp = (1/(0.0014+0.000237*logrt+0.000000099*logrt*logrt*logrt))-284.15;
    lcd.print(temp);
    lcd.print("C  ");
    if(buttons) break;
  }
  return;
}

void clock(){
  i=1;
  DateTime now = rtc.now();
  uint8_t buttons = lcd.readButtons();
  lcd.print("TIME ");
  delay(200);
  while(true){
    buttons = lcd.readButtons(); //this is necessary?  
    lcd.setCursor(0, 1);
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    lcd.print(' ');
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print("      ");
    delay(1000);
    if(buttons) break;
  }
  return;
}

void editTemp(){
  uint8_t buttons = lcd.readButtons();
  lcd.setCursor(0,0);
  lcd.print("ENTER TEMP: ");
  delay(2000);
  while(true){
    buttons = lcd.readButtons();
    if(buttons & BUTTON_LEFT) cookTemp--;
    if(buttons & BUTTON_RIGHT) cookTemp++;
    lcd.setCursor(0,1);
    lcd.print(cookTemp);
    lcd.print("    ");
    delay(100);
    if(buttons & BUTTON_UP) break;
  }
  delay(500);
  return;
}

void editTime(){
  DateTime now = rtc.now();
  uint8_t buttons = lcd.readButtons();
  lcd.setCursor(0,0);
  lcd.print("EDIT TIME");
  int n=0;
  delay(2000);
  while(true){
    buttons = lcd.readButtons();
    if(buttons & BUTTON_LEFT) cookTime--;
    if(buttons & BUTTON_RIGHT) cookTime++;
    lcd.setCursor(0,1);
    lcd.print("H:");
    lcd.print(cookTime/60);
    lcd.print(" M:");
    lcd.print(cookTime%60);
    lcd.print("   ");
    delay(100);
    if(buttons & BUTTON_DOWN) break;
  }
}

//EEPROM.write(cookTime)
//EEPROM.write(cookTemp)

