#include <Wire.h>
//Mega2560 pin 20 (SDA), pin 21 (SCL)
//SPI pin
//MOSI 51
//MISO 50
//SCK 52
//SS 53
#include "TimerOne.h"
#include <Adafruit_SSD1331.h>
///////////////////////////////////////////////////////

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 53

int rfidCheck = 0;

MFRC522 mfrc522;  //Create MFRC522 instance
char *reference;
byte uid[] = {0xFA, 0x1C, 0x4A, 0xB3};  //Required Card UID

///////////////////////////////////////////////////////
//Power detection
#define voltageValuePin A7
float  voltageValue = 0;

int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
int value = 0;
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//Ultra Sonic
#define trigPin 13
#define echoPin 12

int f = 0;
int ok = 1;
unsigned long time = 0;

unsigned long UltraSonicStartTime = 0;
int UltraSonicDone = 1;
int    duration;
float  distance = 10.0;
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//interrupt pin
int interruptL1 = 2;
int interruptL2 = 3;

int interruptR1 = 18;
int interruptR2 = 19;
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//motor driver LM298 control pin
int pin1R = 8;
int pin2R = 9;
int pin1L = 10;
int pin2L = 11;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// encoder parameter
//encoderValue
long encoderValueR = 0;
long encoderValueL = 0;

//Reset encoderValue
int encoderValueR_Reset = 0;
int encoderValueL_Reset = 0;

//encoderValue Diff
long encoderValueR_Diff = 0;
long encoderValueL_Diff = 0;
//////////////////////////////////////////////////////

int forwardStart = 0;
int rightStart = 0;
int leftStart = 0;
int targetValue = 0;

////////////////////////////////////////////
//For black line
int prev_blackLineFR = 0;
int rising_blackLineFR = 0;
int prev_blackLineFL = 0;
int rising_blackLineFL = 0;
int rising_blackLineFR_level = 0;
int rising_blackLineFL_level = 0;
int state = 0; //L = -1, forward = 0, R = 1
////////////////////////////////////////////
//For bluetooth
int state2 = 0;
int counter = 0;

////////////////////////////////////////////
//For coordinate
int x = 0;
int y = 0;

int rotation = 0;

int forward = 0;

//////////////////////////////////////////
//buzzer
const int buzzer = 7;

////////////////////////////////////////////

void setup()
{
  ///////////////////////////

  //RFID module
  Serial.begin(115200);
  SPI.begin();

  mfrc522.PCD_Init(SS_PIN, RST_PIN);  // 初始化MFRC522卡
  Serial.print(F("Reader "));
  Serial.print(F(": "));
  mfrc522.PCD_DumpVersionToSerial();  // 顯示讀卡設備的版本

  /////////////////////////////////////////
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
  Serial3.begin(38400);
  Serial3.println("Hello, world?");


  /////////////////////////////////////////



  /////////////////////////////////////////
  //Ultra Sonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  /////////////////////////////////////////

  /////////////////////////////////////////
  //Motor
  pinMode(pin1L, OUTPUT);
  pinMode(pin2L, OUTPUT);
  pinMode(pin1R, OUTPUT);
  pinMode(pin2R, OUTPUT);
  /////////////////////////////////////////

  /////////////////////////////////////////
  //Encoder
  //https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  pinMode(interruptL1, INPUT_PULLUP);
  pinMode(interruptL2, INPUT_PULLUP);
  pinMode(interruptR1, INPUT_PULLUP);
  pinMode(interruptR2, INPUT_PULLUP);


  ////////////////////////
  //buzzer
  pinMode(buzzer, OUTPUT);

  ////////////////////////
}



/////////////////////////////
//Main Loop
void loop()
{
  if (rfidCheck == 0) {
    rfidReader();

  }

  if (rfidCheck == 1) {
    UltraSonic();
    Beeper();
    delay(10);
    counter +=1;

    if (counter==10){
      state2 = 0;
      counter = 0;
      
    }

    if (Serial3.available()) {

      state2 = Serial3.read();

      if (state2 == '0') {
        stopCar();
        
      }
      if (state2 == '1') {
        forwardCar();
        
      }
      if (state2 == '2') {
        backwardCar();
        
      }
      if (state2 == '3') {
        turnCarOnsiteL();
        
      }
      if (state2 == '4') {
        turnCarOnsiteR();

      }
    }
  }

}

/////////////////////////////
// rfid module
void rfidReader() {
  //Serial.print("reading...");
  // 檢查是不是偵測到新的卡
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // 顯示卡片的UID
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size); // 顯示卡片的UID
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));  //顯示卡片的類型

    //把取得的UID，拿來比對我們指定好的UID
    bool they_match = true; // 初始值是假設為真
    for ( int i = 0; i < 4; i++ ) { // 卡片UID為4段，分別做比對
      if ( uid[i] != mfrc522.uid.uidByte[i] ) {
        they_match = false; // 如果任何一個比對不正確，they_match就為false，然後就結束比對
        break;
      }
    }

    //在監控視窗中顯示比對的結果
    if (they_match) {
      Serial.print(F("Access Granted!"));
      rfidCheck = 1;
      tone(buzzer, 1000, 100);
      delay(100);
    } else {
      Serial.print(F("Access Denied!"));
      rfidCheck = 0;
      tone(buzzer, 5000, 50);
      delay(100);
    }
    mfrc522.PICC_HaltA();  // 卡片進入停止模式
  }
}


//////////////////////////////////////////////////////


void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}






//////////////////////////////////////////////////////
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

//
void forwardRightWheel() {
  digitalWrite(pin1R, 1);
  digitalWrite(pin2R, 0);
}
void forwardLeftWheel() {
  digitalWrite(pin1L, 1);
  digitalWrite(pin2L, 0);
}
void backwardRightWheel() {
  digitalWrite(pin1R, 0);
  digitalWrite(pin2R, 1);
}
void backwardLeftWheel() {
  digitalWrite(pin1L, 0);
  digitalWrite(pin2L, 1);
}
void stopRightWheel() {
  digitalWrite(pin1R, 0);
  digitalWrite(pin2R, 0);
}
void stopLeftWheel() {
  digitalWrite(pin1L, 0);
  digitalWrite(pin2L, 0);
}
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//UltraSonic subroutine


void UltraSonic() {
  if (UltraSonicDone) {
    UltraSonicDone = 0;
    UltraSonicStartTime = millis();
    digitalWrite(trigPin, LOW);  // Added this line
  }
  if (millis() > UltraSonicStartTime + 2) {
    digitalWrite(trigPin, HIGH);
  }
  if (millis() > UltraSonicStartTime + 12) {
    digitalWrite(trigPin, LOW);
    //    duration = pulseIn(echoPin, HIGH, 3000); //set 3000ns as timout
    duration = pulseIn(echoPin, HIGH); //set 3000ns as timout
    distance = (duration / 2) / 29.1;
    UltraSonicDone = 1;
  }
}

void Beeper() {

  if (ok == 1) {
    ok = 0;
    time = millis();

  }
  if (millis() > time + (((distance - 4) * 50))) {
    int f = 600 + (10 - distance) * 200;
    tone(buzzer, f, 10);
    ok = 1;
  }
}
