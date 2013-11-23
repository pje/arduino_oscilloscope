#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

const int analogIn = 0;
const int serialBaudRate = 9600;
unsigned int i = 0;
const int sample_production_rate_hertz = 10;

void setup() {
  Serial.begin(serialBaudRate);
}

void serialWriteVoltage( int voltage) {
  byte byte1 = ((voltage >> CHAR_BIT) & 0xff);
  byte byte2 = (voltage & 0xff);
  byte bytes[2] = { byte1, byte2 };
  Serial.write(bytes, 2);
}

void loop() {
//  int voltage_value = analogRead(analogIn);
  int voltage_value = ++i % 1024;
  serialWriteVoltage(voltage_value);
  delay(100 / sample_production_rate_hertz);
}

