
#include <Wire.h>

// CRC calculation as per:
// https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Sensirion_Mass_Flow_Meters_CRC_Calculation_V1.pdf

#define POLYNOMIAL 0x31     // P(x)=x^8+x^5+x^4+1 = 100110001

uint8_t CRC_prim (uint8_t x, uint8_t crc) {
  crc ^= x;
  for (uint8_t bit = 8; bit > 0; --bit) {
    if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
    else crc = (crc << 1);
  }
  return crc;
}

#define sfm3300i2c 0x40

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while(!Serial) {} // let serial console settle

  Wire.beginTransmission(sfm3300i2c);
  Wire.write(0x20); // soft reset
  Wire.write(0x00); // command 0x2000
  Wire.endTransmission();
  delay(100);

  Wire.beginTransmission(sfm3300i2c);
  Wire.write(0x10);  // start continuous measurement
  Wire.write(0x00);  // command 0x1000
  Wire.endTransmission();

  // discard the first read that is all 0xFF
  delay(100); /*
  Wire.requestFrom(sfm3300i2c,3);
  Wire.read();
  Wire.read();
  Wire.read();*/

  Serial.println("Flow\tVolume");
}


const unsigned mt_delta = 100; // measurement interval in ms
bool crc_error;
float flow;
float vol;

void SFM_measure() {
  if (3 == Wire.requestFrom(sfm3300i2c, 3)) {
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
    vol += flow/60*mt_delta; // flow measured in slm; volume calculated in (s)cc
    // /60 --> convert to liters per second
    // *1000 --> convert liters to cubic centimeters
    // /1000 --> we count time in milliseconds
    // *mt_delta --> current measurement time delta in milliseconds
  } else {
    // report i2c read error
  }
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
  if (ms_curr - ms_prev >= mt_delta) { // "soft interrupt" every mt_delta milliseconds
    ms_prev = ms_curr;
    SFM_measure();
    display_flow_volume();
  }
}
