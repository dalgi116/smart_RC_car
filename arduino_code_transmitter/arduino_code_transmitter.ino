#include <SoftwareSerial.h>

SoftwareSerial HC12(12, 13);
const byte brakePotPin = A0;
const byte gassPotPin = A1;
const byte reverseBtnPin = 0;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  pinMode(brakePotPin, INPUT);
  pinMode(gassPotPin, INPUT);
  pinMode(reverseBtnPin, INPUT);
}

void loop() {
  int brakePotValue = getMappedValue(analogRead(brakePotPin));
  int gassPotValue = getMappedValue(analogRead(gassPotPin));
  byte reverseBtnPushed = digitalRead(reverseBtnPin);

  Serial.println(gassPotValue);
  HC12.write(gassPotValue);
}


int getMappedValue(int pin) {
  int value = analogRead(pin);
  int mappedValue = map(value, 0, 1023, 0, 20);
  return mappedValue;
}
