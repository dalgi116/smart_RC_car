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
  int brakePotValue = map(analogRead(brakePotPin), 0, 1023, 0, 254);
  int gassPotValue = map(analogRead(gassPotPin), 0, 1023, 0, 254);
  byte reverseBtnPushed = digitalRead(reverseBtnPin);
  
  int transmitterData[] = {brakePotValue, gassPotValue, reverseBtnPushed};

  HC12.write(255);
  for (int i = 0; i < 3; i++) {
    Serial.print(transmitterData[i]);
    Serial.print(':');
    HC12.write(transmitterData[i]);
  }
  Serial.println();
  delay(50);
}
