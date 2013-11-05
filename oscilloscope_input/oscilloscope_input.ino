#ifndef CHAR_BIT
  #define CHAR_BIT 8
#endif

const int analogIn = 0;
const char beginHeader = 0xff;
const int serialBaudRate = 28800;

void setup() {
  Serial.begin(serialBaudRate);
}

void serialWrite(int voltage, unsigned long time) {
  int bytesToSend = sizeof(typeof(voltage));
  int byte1 = ((voltage >> CHAR_BIT) & 0xff);
  int byte2 = (voltage & 0xff);
  Serial.write(beginHeader);
  Serial.write(byte1);
  Serial.write(byte2);
}

void loop() {
  serialWrite(analogRead(analogIn), micros());
}

