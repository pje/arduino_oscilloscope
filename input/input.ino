#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

const float pi = 3.141592653;
const int analogIn = 0;
const int serialBaudRate = 9600;

int i = 0;
unsigned char j = '0';

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

//  serialWriteVoltage(analogRead(analogIn));
// <sin for testing>
//  int degree = ++i;
//  double rads = (((degree % 360) * pi) / 180.0); // (0..2pi)
//  double sample = ((sin(rads) + 1.0) / 2.0);     // (0..1)
//  int value = (unsigned int) (sample * 1024.0);
//  Serial.println(value);
//  serialWriteVoltage(value);
// </sin>

  serialWriteVoltage(++i % 1024);
  delay(10);
}

