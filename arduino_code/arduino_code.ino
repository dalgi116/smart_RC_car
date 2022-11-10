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
long unsigned int slowingTime = millis();
byte directionForward = true;
byte directionBtnReady = true;
int brakePotValue;
int gassPotValue;
byte directionBtnPressed;

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
  if (HC12.available()) {
    String transmitterData = HC12.readStringUntil('\n');
    String transmitterDataSplitted[2] = {};
    int transmitterStringCounter = 0;
    while (transmitterData.length() > 0) {
      int spaceIndex = transmitterData.indexOf(' ');
      if (spaceIndex == -1) {
        transmitterDataSplitted[transmitterStringCounter++] = transmitterData;
        break;
      } else {
        transmitterDataSplitted[transmitterStringCounter++] = transmitterData.substring(0, spaceIndex);
        transmitterData = transmitterData.substring(spaceIndex + 1);
      }
    }
    brakePotValue = map(transmitterDataSplitted[0].toInt(), 10, 255, 0, 30);
    gassPotValue = map(transmitterDataSplitted[1].toInt(), 10, 255, 0, 30);
    directionBtnPressed = transmitterDataSplitted[2].toInt();
    
    Serial.print(brakePotValue);
    Serial.print(' ');
    Serial.print(gassPotValue);
    Serial.print(' ');
    Serial.print(directionBtnPressed);
    Serial.println();
    
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
  if (brakePotValue) {
    if (!braking) {
      brakingTime = millis();
      braking = true;
    }
    if (millis() >= brakingTime) {
      actualSpeed -= brakePotValue;
      brakingTime += 150;
    }
  } else if (gassPotValue) {
    if (!accelerating) {
      acceleratingTime = millis();
      accelerating = true;
    }
    if (millis() >= acceleratingTime) {
      actualSpeed += gassPotValue;
      acceleratingTime += 200;
    }
  }
  if (millis() >= slowingTime) {
    actualSpeed -= 1;
    slowingTime += 100;
  }
  actualSpeed = repairMotorSpeed(actualSpeed);
  if (actualSpeed == motorsLowSignal) {
    motorsStop();
    if (directionBtnPressed && directionBtnReady) {
      if (directionForward) {
        directionForward = false;
      } else {
        directionForward = true;
      }
      directionBtnReady = false;
    }
  } else {
    if (directionForward) {
      motorsForward(actualSpeed); 
    } else {
      motorsBackward(actualSpeed);
    }
  }
}

int repairMotorSpeed(int actualSpeed) {
  if (actualSpeed <= motorsLowSignal) {
    actualSpeed = motorsLowSignal;
  } else if (actualSpeed > 255) {
    actualSpeed = 255;
  }
  return actualSpeed;
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
