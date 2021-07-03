#include <Wire.h>

//use IIC2 STM32
TwoWire Wire2 (2, I2C_FAST_MODE);
#define Wire Wire2

void setup() {
  Serial.begin(115200);
  Wire.setClock(400000);
  Wire.begin();
}

void loop() {
  byte error, address;
  int nDevices;

  nDevices = 0;
  for(address = 1; address < 127; address++) {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      
      nDevices++;
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  }
  else {
    // два байта из парного регистра
    Wire.beginTransmission(0x55);
    Wire.write(0x08);
    Wire.endTransmission();
    
    Wire.requestFrom(0x55, 2);
  
    if (Wire.available() >= 1 )
    {
      uint8_t f = Wire.read();
      Serial.print("voltage: ");
      Serial.print(((Wire.read() << 8) | f) * 0.001);
      Serial.print(" V");
    }
    Wire.endTransmission();
    Serial.println();

    Wire.beginTransmission(0x55);
    Wire.write(0x14);
    Wire.endTransmission();
    
    Wire.requestFrom(0x55, 2);
  
    if (Wire.available() > 0 ) {
      uint8_t f = Wire.read();
      Serial.print("current: ");
      Serial.print(((short)((Wire.read() << 8) | f)) * 0.001);
      Serial.print(" A");
    }
    Wire.endTransmission();
    Serial.println();
  
    // порцию байт по ключевому запросу
    Wire.beginTransmission(0x55);
    Wire.write(0x3F);
    Wire.write(0x03); // 0x01 0x02 0x03
    Wire.endTransmission();
    
    Wire.beginTransmission(0x55);
    Wire.write(0x40);
    Wire.endTransmission();
  
    Wire.requestFrom(0x55, 32);
  
    char ddd[32];
    int i = 0;
  
    while (Wire.available() > 0) {
      ddd[i] = (char)Wire.read();
      i++;
    }
    Wire.endTransmission();
    Serial.print("manufacturer block 1: ");
    Serial.print(ddd);
    Serial.println();
    
    // пару байт из регистра контроля
    Wire.beginTransmission(0x55);
    Wire.write(0x00);
    Wire.write(0x01);
    Wire.endTransmission();
    Wire.beginTransmission(0x55);
    Wire.write(0x01);
    Wire.write(0x00);
    Wire.endTransmission();
  
    Wire.beginTransmission(0x55);
    Wire.write(0x00);
    Wire.endTransmission();
  
    Wire.requestFrom(0x55, 2);

    if (Wire.available() > 0 ) {
      uint8_t f = Wire.read();
      Serial.print("ic model: ");
      Serial.print((Wire.read() << 8) | f, HEX);
    }
    Wire.endTransmission();
    Serial.println();
  }
  
  Serial.println();
  delay(3000);
}
