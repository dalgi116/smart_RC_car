#include <SoftwareSerial.h>

SoftwareSerial HC12(12, 13);
const byte motorA1Pin = 7;
const byte motorA2Pin = 2;
const byte motorB1Pin = 3;
const byte motorB2Pin = 4;
const byte motorASpeedControlPin = 5;
const byte motorBSpeedControlPin = 6;

int motorsLowSignal = 45;
int actualSpeed = 0;
bool braking = false;
long unsigned int brakingTime;
bool accelerating = false;
long unsigned int acceleratingTime;
bool slowing = false;
long unsigned int slowingTime;
byte directionForward = true;
byte directionSwap = false;
byte directionBtnReady = true;
byte recievedValue;
int brakePotValue;
int gassPotValue;
bool directionBtnPressed;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  
  pinMode(motorA1Pin, OUTPUT);
  pinMode(motorA2Pin, OUTPUT);
  pinMode(motorB1Pin, OUTPUT);
  pinMode(motorB2Pin, OUTPUT);
  pinMode(motorASpeedControlPin, OUTPUT);
  pinMode(motorBSpeedControlPin, OUTPUT);
}


void loop() {
  while (HC12.available()) {
    String transmitterData = HC12.readStringUntil('\n');
    Serial.println(transmitterData);
    
  /*
    int gassPotValue = 0;
    int brakePotValue = 0;
    byte directionBtnPressed = false;
  
    if (directionBtnPressed && directionBtnReady) {
      directionSwap = true;
      directionBtnReady = false;
    }
    if (!directionBtnPressed) {
      directionBtnReady = true;
    }  
  
    if (!brakePotValue) {
      braking = false;
    }
    if (!gassPotValue) {
      accelerating = false;
    }
    if (gassPotValue || brakePotValue) {
      slowing = false;
    }
    
    if (brakePotValue) {
      if (!braking) {
        brakingTime = millis();
        braking = true;
      }
      if (millis() >= brakingTime && actualSpeed >= motorsLowSignal) {
        actualSpeed -= brakePotValue;
        brakingTime += 150;
        if (actualSpeed < motorsLowSignal) {
          actualSpeed = motorsLowSignal;
        }
      }
    } else if (gassPotValue) {
      if (!accelerating) {
        acceleratingTime = millis();
        accelerating = true;
      }
      if (millis() >= acceleratingTime && actualSpeed <= 255) {
        actualSpeed += gassPotValue;
        acceleratingTime += 200;
        if (actualSpeed > 255) {
          actualSpeed = 255;
        }
      }
    } else {
        if (!slowing) {
        slowingTime = millis();
        slowing = true;
      }
      if (millis() >= slowingTime && actualSpeed >= motorsLowSignal) {
        actualSpeed -= 1;
        slowingTime += 40;
        if (actualSpeed < motorsLowSignal) {
          actualSpeed = motorsLowSignal;
        }
      }
    }
  
    if (actualSpeed <= motorsLowSignal) {
      motorsStop();
      if (directionSwap) {
        if (directionForward) {
          directionForward = false;
        } else {
          directionForward = true;
        }
      }
    } else {
      if (directionForward) {
        motorsForward(actualSpeed); 
      } else {
        motorsBackward(actualSpeed);
      }
    }
  */
  }
}

void motorsForward(int speedValue) {  //min 45
  digitalWrite(motorA1Pin, HIGH);
  digitalWrite(motorA2Pin, LOW);
  analogWrite(motorASpeedControlPin, speedValue);
  digitalWrite(motorB1Pin, HIGH);
  digitalWrite(motorB2Pin, LOW);
  analogWrite(motorBSpeedControlPin, speedValue);
}

void motorsStop() {
  digitalWrite(motorA1Pin, LOW);
  digitalWrite(motorA2Pin, LOW);
  analogWrite(motorASpeedControlPin, LOW);
  digitalWrite(motorB1Pin, LOW);
  digitalWrite(motorB2Pin, LOW);
  analogWrite(motorBSpeedControlPin, LOW);
}

void motorsBackward(int speedValue) {  //min 45
  digitalWrite(motorA1Pin, LOW);
  digitalWrite(motorA2Pin, HIGH);
  analogWrite(motorASpeedControlPin, speedValue);
  digitalWrite(motorB1Pin, LOW);
  digitalWrite(motorB2Pin, HIGH);
  analogWrite(motorBSpeedControlPin, speedValue);
}
