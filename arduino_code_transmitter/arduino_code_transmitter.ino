#include <SoftwareSerial.h>

SoftwareSerial HC12(12, 13);
const byte brakePotPin = A0;
const byte gassPotPin = A1;
const byte reverseBtnPin = 2;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  pinMode(brakePotPin, INPUT);
  pinMode(gassPotPin, INPUT);
  pinMode(reverseBtnPin, INPUT);
}

void loop() {
  int brakePotValue = map(analogRead(brakePotPin), 0, 1023, 0, 253);
  int gassPotValue = map(analogRead(gassPotPin), 0, 1023, 0, 253);
  byte reverseBtnPushed = digitalRead(reverseBtnPin);

  String transmitterData = String(brakePotValue) + " " + String(gassPotValue) + " " + String(reverseBtnPushed) + '\n';
  Serial.println(transmitterData);
  HC12.print(transmitterData);
  delay(50);
}
