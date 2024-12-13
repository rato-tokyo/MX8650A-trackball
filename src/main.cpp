#include <Arduino.h>
#include <Mouse.h>
#define SCLK PB9
#define SDIO PB8
#define MOTION_STATUS 0x02
#define Delta_X 0x03
#define Delta_Y 0x04
#define CONFIG 0x06
#define CPI_800 0x85
#define CPI_1000 0x86
#define CPI_1200 0x87
#define CPI_1600 0x88

void writeRegister(uint8_t address, uint8_t data)
{
  int i = 7;
  address |= 0x80;

  pinMode(SDIO, OUTPUT);
  for (; i >= 0; i--)
  {
    digitalWrite(SCLK, LOW);
    digitalWrite(SDIO, address & (1 << i));
    digitalWrite(SCLK, HIGH);
  }

  for (i = 7; i >= 0; i--)
  {
    digitalWrite(SCLK, LOW);
    digitalWrite(SDIO, data & (1 << i));
    digitalWrite(SCLK, HIGH);
  }
}

uint8_t readRegister(uint8_t address)
{
  int i = 7;
  uint8_t r = 0;

  pinMode(SDIO, OUTPUT);
  for (; i >= 0; i--)
  {
    digitalWrite(SCLK, LOW);
    digitalWrite(SDIO, address & (1 << i));
    digitalWrite(SCLK, HIGH);
  }

  pinMode(SDIO, INPUT);

  for (i = 7; i >= 0; i--)
  {
    digitalWrite(SCLK, LOW);
    digitalWrite(SCLK, HIGH);
    r |= (digitalRead(SDIO) << i);
  }

  return r;
}

void setup()
{
  Serial.begin(9600);
  Mouse.begin();
  pinMode(SCLK, OUTPUT);
  digitalWrite(SCLK, HIGH);
  pinMode(SDIO, INPUT);
  digitalWrite(SDIO, LOW);
  writeRegister(CONFIG, CPI_800);

  writeRegister(0x09, 0x5A);
  writeRegister(0x0D, 0x04);

}

void loop()
{
  if (readRegister(MOTION_STATUS) & 0x80)
  {
    Mouse.move(
      (signed char)readRegister(Delta_X),
      (signed char)readRegister(Delta_Y)
      );
  }
 delay(10);
}