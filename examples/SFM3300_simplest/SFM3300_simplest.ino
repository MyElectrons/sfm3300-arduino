
#include <Wire.h>

/* Inspired by :
 * https://stackoverflow.com/questions/60741196/difference-i2c-sensor-reading-raspberry-pi-and-arduino
 */
#define sfm3300i2c 0x40

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while(!Serial) {} // let serial console settle
  
  Wire.beginTransmission(sfm3300i2c);
  Wire.write(0x10); // start continuous measurement
  Wire.write(0x00); // command 0x1000
  Wire.endTransmission();
}

void loop() {
  delay(100);
  
  if (2 == Wire.requestFrom(sfm3300i2c, 2)) { // just keep reading SLM (Standard Liter per Minute)
    uint16_t a = Wire.read(); // only two bytes need to be read
    uint8_t  b = Wire.read(); // if we don't care about CRC
    a = (a<<8) | b;
    float flow = ((float)a - 32768) / 120;
    Serial.println(flow);
  }
}
