#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);
Adafruit_ADS1115 ads2(0x4A);
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
byte degree[8] = { 0b00110, 0b01001, 0b01001, 0b00110, 0b00000, 0b00000, 0b00000, 0b00000 };

float R1 = 110000.0, R2 = 11000.0, R3 = 110000.0, R4 = 11000.0;
float value1 = 0.0, value2 = 0.0, value3 = 0.0, value4 = 0.0, value5 = 0.0, vout1 = 0.0, vout2 = 0.0, vout3 = 0.0;
float vin1 = 0.0, vin2 = 0.0, vmax1 = 0.0, vmax2 = 0.0, temp1 = 0.0, temp2 = 0.0;
float average1 = 0.0, average2 = 0.0, average3 = 0.0, average4 = 0.0, average5 = 0.0;
float vmin1 = 20.0, vmin2 = 20.0;
int avg = 10, current = 0, currentmax = 0, power = 0, powermax = 0;
int16_t adc0, adc1, adc2, adc20, adc21;

void setup()
{
  ads.begin();
  ads2.begin();
  lcd.begin(16,4);
  lcd.createChar(1, degree);
}

void loop()
{
  adc0 = ads.readADC_SingleEnded(0); adc1 = ads.readADC_SingleEnded(1); adc2 = ads.readADC_SingleEnded(2);
  adc20 = ads2.readADC_SingleEnded(1); adc21 = ads2.readADC_SingleEnded(2);
  value1 = adc2; value2 = adc1; value3 = adc0;
  temp1 = (adc20 * 0.1875)/10;
  temp2 = (adc21 * 0.1875)/10;

/*
  average1 = 0.0; average2 = 0.0; average3 = 0.0; average4 = 0.0; average5 = 0.0;

  for (int i=0; i < avg; i++) {
    average1 = average1 + ads.readADC_SingleEnded(0);
    average2 = average2 + ads.readADC_SingleEnded(1);
    average3 = average3 + ads.readADC_SingleEnded(2);
    average4 = average4 + ads2.readADC_SingleEnded(1);
    average5 = average5 + ads2.readADC_SingleEnded(2);
  }

  value1 = average2/avg; value2 = average3/avg; value3 = average1/avg; value4 = average4/avg; value5 = average5/avg;
  temp1 = (value4 * 0.1875)/10; temp2 = (value5 * 0.1875)/10;
*/

  vout1 = (value1 * 0.1875)/1000;
  vin1 = vout1 / (R2/(R1+R2));
  if (vin1 < 0.09) { vin1 = 0.0; }
  if (vin1 < vmin1) { vmin1 = vin1; }
  if (vin1 > vmax1) { vmax1 = vin1; }
  vout2 = (value2 * 0.1875)/1000;
  vin2 = vout2 / (R4/(R3+R4));
  if (vin2 < 0.09) { vin2 = 0.0; }
  if (vin2 < vmin2) { vmin2 = vin2; }
  if (vin2 > vmax2) { vmax2 = vin2; }
  vout3 = (value3 * 0.1875)/1000;
  current = (vout3 - 2.541) / 0.02;
  if (current < 0.1) { current = 0; }
  if (current > currentmax) { currentmax = current; }
  power = vin2 * current / 2;
  if (power > powermax) { powermax = power; }
  lcd.setCursor(0, 0);  lcd.print(vmin1, 1);
  lcd.setCursor(0, 1);  lcd.print(vmin2, 1);
  lcd.setCursor(6, 0);  lcd.print(vin1, 1);
  lcd.setCursor(6, 1);  lcd.print(vin2, 1);
  lcd.setCursor(12, 0); lcd.print(vmax1, 1);
  lcd.setCursor(12, 1); lcd.print(vmax2, 1);
  lcd.setCursor(0,2);   lcd.print(round(temp1)); lcd.print(char(1)); lcd.print("C"); lcd.print(" ");
  lcd.setCursor(6,2);   lcd.print(current, 1); lcd.print("A  ");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
  lcd.setCursor(12,2);  lcd.print(currentmax); lcd.print("A  ");
  lcd.setCursor(0,3);   lcd.print(round(temp2)); lcd.print(char(1)); lcd.print("C"); lcd.print(" ");
  lcd.setCursor(6,3);   lcd.print(power);      lcd.print("W  ");
  lcd.setCursor(12,3);  lcd.print(powermax);   lcd.print("W  ");
  delay(400);
}