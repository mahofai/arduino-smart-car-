#include <SPI.h>
#include <MFRC522.h>

// 3.3Vv
#define RST     33
// GND
#define MISO    36
#define MOSI    33
#define SCK     37
#define SDA     34



MFRC522 mfrc522;

void setup() {

  Serial.begin(115200);

  SPI.begin():

  mfrc522.PCD_Init(
}
