#ifndef CHAR_BIT
  #define CHAR_BIT 8
#endif

const int analogIn = 0;
const int gateOut = 10;
const int triggerOut = 11;

const char beginHeader = 0xff;
const int serialBaudRate = 28800;
const int period = 2000;
const int triggerDuration = 20;
const int gateDuration = 400;
const int triggerVoltageOn = LOW;
const int triggerVoltageOff = HIGH;
const int gateVoltageOn = HIGH;
const int gateVoltageOff = LOW;
unsigned long cyclePosition = 0;

void setup() {
  pinMode(gateOut, OUTPUT);
  pinMode(triggerOut, OUTPUT);
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

