
#include <Wire.h>

/* Inspired by :
 * https://stackoverflow.com/questions/60741196/difference-i2c-sensor-reading-raspberry-pi-and-arduino
 */

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(500); // let serial console settle
  Wire.beginTransmission(0x40); // start continuous measurement
  Wire.write(0x10);
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop() {
  delay(100);
  Wire.requestFrom(0x40, 2); // just keep reading SLM (Standard Liter per Minute)
  uint16_t a = Wire.read();
  uint8_t  b = Wire.read();
  a = (a<<8) | b;
  float flow = ((float)a - 32768) / 120;
  Serial.println(flow);
}
