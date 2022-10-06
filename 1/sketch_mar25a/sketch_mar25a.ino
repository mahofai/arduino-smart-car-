#include <Wire.h>
//Mega2560 pin 20 (SDA), pin 21 (SCL)
//SPI pin
// MOSI 51
//MISO 50
//SCK 52
//SS 53
#include <Adafruit_SSD1331.h>
///////////////////////////////////////////////////////
//OLED
int sclk = 52; //brown--- connect this to the display module CLK pin (Serial Clock)
int mosi = 51; //orange--- connect this to the display module DIN pin (Serial Data)
int rst  = 49; //yellow--- connect this to the display module RES pin (Reset)
int dc   = 50; //green--- connect this to the display module D/C  pin (Data or Command)
int cs   = 48; //blue--- connect this to the display module CS  pin (Chip Select)
// Color definitions

#define BLACK          0x0000
#define BLUE            0x0006
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF
#define BACKGROUND      0x0000
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);


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
long encoderValueR = 0;
long encoderValueL = 0;
//////////////////////////////////////////////////////

int forwardStart = 0;
int targetValue = 0;

void setup()
{
  //Serial Setup
  Serial.begin(115200);

  /////////////////////////////////////////
  //Motor
  pinMode(pin1L, OUTPUT);
  pinMode(pin2L, OUTPUT);
  pinMode(pin1R, OUTPUT);
  pinMode(pin2R, OUTPUT);
  /////////////////////////////////////////

  //OLED
  display.begin();
  display.fillScreen(BLACK);
  tftPrintTest();
  delay(2000);

  /////////////////////////////////////////
  //Encoder
  //https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  pinMode(interruptL1, INPUT_PULLUP);
  pinMode(interruptL2, INPUT_PULLUP);
  pinMode(interruptR1, INPUT_PULLUP);
  pinMode(interruptR2, INPUT_PULLUP);
  //setup interrupt
  attachInterrupt(digitalPinToInterrupt(interruptL1), countL, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptR1), countR, FALLING);
  /////////////////////////////////////////
}

/////////////////////////////
//Main Loop
void loop()
{
  MotorControl();
    Serial.print(" encoderValueL= ");
    Serial.print(encoderValueL);
    Serial.print(" encoderValueR= ");
    Serial.print(encoderValueR);
    Serial.println("");
}

//////////////////////////////////////////////////////
//Motor control
void MotorControl() {
    forward_stline();
}

//////////////////////////////////////////
void forward_stline() {
  //forward 100 click
  if (forwardStart == 0) {
    stopCar();
    encoderValueL = 0;
    encoderValueR = 0;
    forwardStart = 1;
  }
  if (encoderValueL > targetValue or encoderValueR > targetValue) {
    stopCar();
    forwardStart = 0;
  }
  else {
    turnCarOnsiteL();
  }
}

//////////////////////////////////////////////////////
//Interrupt subroutine
void countL() {
  if (digitalRead(interruptL2)) {
    encoderValueL--;
  }
  else {
    encoderValueL++;
  }
}
void countR() {
  if (digitalRead(interruptR2)) {
    encoderValueR++;
  }
  else {
    encoderValueR--;
  }
}
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////

void tftPrintTest() {
  display.fillScreen(BLACK);
  display.setCursor(15, 5);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.print("encoderValueR= ");
  display.print(encoderValueR);
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
