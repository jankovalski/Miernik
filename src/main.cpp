#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

float vout1 = 0.0; float vout2 = 0.0; float vout3 = 0.0;
float vin1 = 0.0; float vin2 = 0.0;
float R1 = 110000.0; float R2 = 11000.0; float R3 = 110000.0; float R4 = 11000.0;
float value1 = 0.0; float value2 = 0.0; float value3 = 0.0;
float vmin1 = 20.0; float vmin2 = 20.0; float vmax1 = 0.0; float vmax2 = 0.0;
float current; int currentmin; int currentmax;
int avg = 10; int power; int powermin; int powermax;

void setup()
{
  ads.begin();
  lcd.begin(16,4);
}

void loop()
{
  int16_t adc0; int16_t adc1; int16_t adc2;
  adc0 = ads.readADC_SingleEnded(0); adc1 = ads.readADC_SingleEnded(1); adc2 = ads.readADC_SingleEnded(2);
  float average1 = 0; float average2 = 0; float average3 = 0;
  for (int i=0; i < avg; i++) {
    average1 = average1 + ads.readADC_SingleEnded(0);
    average2 = average2 + ads.readADC_SingleEnded(1);
    average3 = average3 + ads.readADC_SingleEnded(2);
  }
  value1 = average1/avg; value2 = average2/avg; value3 = average3/avg;
  value1 = adc1; value2 = adc2; value3 = adc0;
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
  vout3 = (adc0 * 0.1875)/1000;
  current = (vout3 - 2.541) / 0.02;
  if (current < 0.1) { current = 0; }
  if (current < currentmin) { currentmin = current; }
  if (current > currentmax) { currentmax = current; }
  power = vin2 * current / 2;
  if (power < powermin) { powermin = power; }
  if (power > powermax) { powermax = power; }
  lcd.setCursor(0, 0);  lcd.print(vmin1, 1);
  lcd.setCursor(0, 1);  lcd.print(vmin2, 1);
  lcd.setCursor(6, 0);  lcd.print(vin1, 1);
  lcd.setCursor(6, 1);  lcd.print(vin2, 1);
  lcd.setCursor(12, 0); lcd.print(vmax1, 1);
  lcd.setCursor(12, 1); lcd.print(vmax2, 1);
  lcd.setCursor(0,2);   lcd.print(currentmin); lcd.print("A  ");
  lcd.setCursor(6,2);   lcd.print(current, 1); lcd.print("A  ");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
  lcd.setCursor(12,2);  lcd.print(currentmax); lcd.print("A  ");
  lcd.setCursor(0,3);   lcd.print(powermin);   lcd.print("W  ");
  lcd.setCursor(6,3);   lcd.print(power);      lcd.print("W  ");
  lcd.setCursor(12,3);  lcd.print(powermax);   lcd.print("W  ");
  delay(400);
}