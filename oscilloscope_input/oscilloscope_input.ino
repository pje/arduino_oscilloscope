#ifndef CHAR_BIT
  #define CHAR_BIT 8
#endif

const int analogIn = 0;
const char beginHeader = 0xff;
const int serialBaudRate = 28800;

void setup() {
  Serial.begin(serialBaudRate);
}

void readAnalogIn() {
  int voltageValue = analogRead(analogIn);
  int bytesToSend = sizeof(typeof(voltageValue));
  int byte1 = ((voltageValue >> CHAR_BIT) & 0xff);
  int byte2 = (voltageValue & 0xff);
  Serial.write(beginHeader);
  Serial.write(byte1);
  Serial.write(byte2);
}

void loop() {
  readAnalogIn();
}

