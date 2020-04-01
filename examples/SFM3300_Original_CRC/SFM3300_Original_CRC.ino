
#include <Wire.h>


// CRC calculation as per:
// https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Sensirion_Mass_Flow_Meters_CRC_Calculation_V1.pdf

typedef enum {
  CHECKSUM_ERROR = 0X04
} etError;
#define POLYNOMIAL 0x131
//P(x)=x^8+x^5+x^4+1 = 100110001
//============================================================
uint8_t SF04_CheckCrc (uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
//============================================================
//calculates checksum for n bytes of data
//and compares it with expected checksum
//input:       data[]      checksum is built based on this data
//             nbrOfBytes  checksum is built for n bytes of data
//             checksum    expected checksum
//return:      error:      CHECKSUM_ERROR = checksum does not match
//                         0 = checksum matches
//============================================================
{
uint8_t crc = 0;
uint8_t byteCtr;
//calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr) {
    crc ^= (data[byteCtr]);
    for (uint8_t bit = 8; bit > 0; --bit) {
      if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
      else crc = (crc << 1);
    }
  }
  if (crc != checksum) return CHECKSUM_ERROR;
  else return 0;
}


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(500); // let serial console settle
  Wire.beginTransmission(byte(0x40));
  Wire.write(byte(0x10));
  Wire.write(byte(0x00));
  Wire.endTransmission();
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  Wire.requestFrom(0x40,3);
  uint8_t rd[2];
  uint16_t a = rd[0] = Wire.read();
  uint8_t  b = rd[1] = Wire.read();
  uint8_t  c = Wire.read();
  a = (a<<8) | b;
  float flow = ((float)a - 32768) / 120;
  Serial.print(flow);
  if (SF04_CheckCrc(rd, 2, c))
    Serial.println(" CRC error");
  else
    Serial.println();
}
