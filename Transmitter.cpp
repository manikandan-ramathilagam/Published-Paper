#include <SoftwareSerial.h>
#include <Gpsneo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Gpsneo gps;
//Gpsneo gps(11,12);

char latitud[11];
char latitudHemisphere[3];
char longitud[11];
char longitudMeridiano[3];

Adafruit_MPU6050 mpu;

SoftwareSerial ss(5, 6);
int j = 0;

#define start
#define SSID "Project"        // "SSID-WiFiname"
#define PASS "12345678"       // "password"
#define IP "184.106.153.149"  // thingspeak.com ip
String msg = "GET /update?key=Q6R6JRFOMYUN9V3Z";

int countTrueCommand;
int countTimeCommand;

boolean found = false;

float m = 11.781;
float n = 70.258;
String Status = "";
#define debug Serial

void updateTemp() {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  ss.println(cmd);
  delay(2000);
  if (ss.find("Error")) {
    return;
  }

  cmd = msg;
  cmd += "&field1=";  // field 1 for temperature
  cmd += String(m);
  cmd += "&field2=";  // field 2 for temperature
  cmd += String(n);
  cmd += "&field3=";  // field 3 for temperature
  cmd += String(Status);
  cmd += "\r\n";

  ss.print("AT+CIPSEND=");
  ss.println(cmd.length());
  delay(2000);

  if (ss.find(">")) {
    ss.print(cmd);
    delay(1000);
    Serial.println("Succeed");
  } else {
    ss.print(cmd);
    delay(1000);
    Serial.println("Succeed");
  }
}

// STEP 2
boolean connectWiFi() {
  ss.println("AT+CWMODE=1");
  delay(2000);

  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  ss.println(cmd);
  delay(5000);

  if (ss.find("OK")) {
    return true;
  } else {
    return false;
  }
}

void setup() {
  Serial.begin(9600);
  mpu.begin();
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  pinMode(7, INPUT);
  pinMode(A0, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  ss.begin(115200);

  // put your setup code here, to run once:
  ss.println("AT");
  delay(2000);
  if (ss.find("OK")) {
    // STEP 2
    connectWiFi();
  }
  delay(3000);
}

// STEP 3
void loop() {
  gps.getDataGPRMC(latitud, latitudHemisphere, longitud, longitudMeridiano);
  Serial.println(latitud);

  int c = digitalRead(7);
  Serial.print(c);

  int b = analogRead(A0);
  Serial.print(b);

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  delay(1000);
  delay(1000);

  if (a.acceleration.x > 0.40) {
    digitalWrite(9, HIGH);
    delay(5000);
    updateTemp();
    digitalWrite(9, LOW);
    Status = "ACCIDENT_OCCURED";
    digitalWrite(8, HIGH);
    updateTemp();
    digitalWrite(8, LOW);
  }

  if (c == 1) {
    digitalWrite(9, HIGH);
    delay(5000);
    digitalWrite(9, LOW);
    Status = "ACCIDENT_OCCURED";
    digitalWrite(8, HIGH);
    updateTemp();
    digitalWrite(8, LOW);
  }

  if (b > 800) {
    Status = "Driver_Drunken";
    updateTemp();
    digitalWrite(9, HIGH);
    delay(5000);
    digitalWrite(9, LOW);
  }
}
