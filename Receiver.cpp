// RECEIVER

#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LCD_I2C.h>

const char ssid[] = "Project";   // your network SSID (name)
const char pass[] = "12345678";  // your network password

WiFiClient client;

unsigned long counterChannelNumber = 1012647;  // Channel ID

const char *myCounterReadAPIKey = "3RLNDT4FRR965PO5";  // Read API Key
const int FieldNumber1 = 1;  // The field you wish to read
const int FieldNumber2 = 2;  // The field you wish to read

LCD_I2C lcd(0x3f);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  pinMode(D0, INPUT);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("AMBULANCE STATION");
  delay(5000);
  lcd.clear();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println();

    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      delay(5000);
    }

    Serial.println("Connected to Wi-Fi Succesfully.");
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  int a = digitalRead(D0);
  Serial.println(a);

  if (a == 1) {
    lcd.setCursor(0, 0);
    lcd.print("ACCIDENT OCCURED");
    lcd.setCursor(0, 1);
    lcd.print("DETECTED");
    delay(1000);
    lcd.clear();

    long temp1 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
    float statusCode1 = ThingSpeak.getLastReadStatus();
    lcd.setCursor(0, 0);
    lcd.print("LAT");
    lcd.setCursor(0, 1);
    lcd.print(temp1);
    delay(1000);
    lcd.clear();

    long temp2 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
    float statusCode2 = ThingSpeak.getLastReadStatus();
    lcd.setCursor(0, 0);
    lcd.print("LONG");
    lcd.setCursor(0, 1);
    lcd.print(temp2);
    delay(1000);
    lcd.clear();

    a = 0;
  }
}
