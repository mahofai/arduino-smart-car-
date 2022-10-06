#include <SoftwareSerial.h>
SoftwareSerial BT(16, 17); // TX,RX
#include <Wire.h>
int bt_value;
int pin1R = 8;
int pin2R = 9;
int pin1L = 10;
int pin2L = 11;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
    //Motor
  pinMode(pin1L, OUTPUT);
  pinMode(pin2L, OUTPUT);
  pinMode(pin1R, OUTPUT);
  pinMode(pin2R, OUTPUT);

void loop() {
  // put your main code here, to run repeatedly:
  if (BT.available()>0){
    bt_value = BT.read();
    switch(bt_value){
      case 'f':
        forwardCar();
       break;
       case 'b':
        backwardCar();
       break;
       case "l":
        turnCarOnsiteL();
       break;
       case "r":
        turnCarOnsiteR();
       break;
       case "s":
        stopCar();
       break;
       default:
       break;
    }
  }
}

// Motor subrountine
void turnCarL() {
  digitalWrite(pin1R, 1);
  digitalWrite(pin2R, 0);
  digitalWrite(pin1L, 0);
  digitalWrite(pin2L, 0);
}
void turnCarR() {
  digitalWrite(pin1R, 0);
  digitalWrite(pin2R, 0);
  digitalWrite(pin1L, 1);
  digitalWrite(pin2L, 0);
}
void turnCarOnsiteL() {
  digitalWrite(pin1R, 1);
  digitalWrite(pin2R, 0);
  digitalWrite(pin1L, 0);
  digitalWrite(pin2L, 1);
}
void turnCarOnsiteR() {
  digitalWrite(pin1R, 0);
  digitalWrite(pin2R, 1);
  digitalWrite(pin1L, 1);
  digitalWrite(pin2L, 0);
}
void forwardCar() {
  digitalWrite(pin1R, 1);
  digitalWrite(pin2R, 0);
  digitalWrite(pin1L, 1);
  digitalWrite(pin2L, 0);
}
void backwardCar() {
  digitalWrite(pin1R, 0);
  digitalWrite(pin2R, 1);
  digitalWrite(pin1L, 0);
  digitalWrite(pin2L, 1);
}
void stopCar() {
  digitalWrite(pin1R, 0);
  digitalWrite(pin2R, 0);
  digitalWrite(pin1L, 0);
  digitalWrite(pin2L, 0);
}
}
