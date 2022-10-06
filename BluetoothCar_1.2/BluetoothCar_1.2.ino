#include <Wire.h>

int state = 0;
int pin1R = 8;
int pin2R = 9;
int pin1L = 10;
int pin2L = 11;

const int buzzer = 7;

////////////////////////////////////

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
//////////////////////////////////////////////////////////////////////

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  Serial3.begin(38400);
  Serial3.println("Hello, world?");
}

void loop() { // run over and over
  if (rfidCheck == 0) {
    rfidReader();

  }
  if (rfidCheck == 1) {
    if (Serial3.available()) {
      state = Serial3.read();

      if (state == '0') {
        stopCar();
      }
      if (state == '1') {
        forwardCar();
      }
      if (state == '2') {
        backwardCar();
      }
      if (state == '3') {
        turnCarOnsiteL();
      }
      if (state == '4') {
        turnCarOnsiteR();
      }
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
