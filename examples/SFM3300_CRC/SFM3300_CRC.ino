
#include <Wire.h>

// CRC calculation as per:
// https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Sensirion_Mass_Flow_Meters_CRC_Calculation_V1.pdf

#define POLYNOMIAL 0x31     //P(x)=x^8+x^5+x^4+1 = 100110001

uint8_t CRC_prim (uint8_t x, uint8_t crc) {
  crc ^= x;
  for (uint8_t bit = 8; bit > 0; --bit) {
    if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
    else crc = (crc << 1);
  }
  return crc;
}


void setup() {

  Wire.begin();
  Serial.begin(115200);
  delay(500); // let serial console settle

  // soft reset
  Wire.beginTransmission(0x40);
  Wire.write(0x20);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(100);


  // start continuous measurement
  Wire.beginTransmission(0x40);
  Wire.write(0x10);
  Wire.write(0x00);
  Wire.endTransmission();

  // discard the first read that is all 0xFF
  delay(100); /*
  Wire.requestFrom(0x40,3);
  Wire.read();
  Wire.read();
  Wire.read();*/

  Serial.println("Flow\tVolume");
}


const unsigned mms = 100; // measurement interval in ms
bool crc_error;
float flow;
float vol;

void SFM_measure() {
  Wire.requestFrom(0x40,3);
  uint8_t crc = 0;
  uint16_t a = Wire.read();
  crc = CRC_prim (a, crc);
  uint8_t  b = Wire.read();
  crc = CRC_prim (b, crc);
  uint8_t  c = Wire.read();
  if (crc_error = (crc != c))
    return;
  a = (a<<8) | b;
  flow = ((float)a - 32768) / 120;
  vol += flow/60*mms; // flow measured in slm; volume calculated in (s)cc
  // /60 --> convert to liters per second
  // *1000 --> convert liters to cubic centimeters
  // /1000*mms --> measurements per second
}


void display_flow_volume() {
    Serial.print(flow);
    Serial.print("\t");
    Serial.print(vol);
    Serial.println(crc_error?" CRC error":"");
}


unsigned long ms_prev = millis(); // timer for measurements "soft interrupts"

void loop() {
  unsigned long ms_curr = millis();
  if (ms_prev + mms < ms_curr) { // "soft interrupt" every mms milliseconds
    ms_prev = ms_curr;
    SFM_measure();
    display_flow_volume();
  }

}
