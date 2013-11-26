#ifndef NBBY
#define NBBY 8
#endif

const int analogIn = 0;
const int serialBaudRate = 9600;
const int sample_production_rate_hertz = 1000;
const int voltage_max_value = 1024;

// protocol
const byte end_of_sample_frame_byte = 0xff;
const int sample_frame_size = 3;

void setup() {
  Serial.begin(serialBaudRate);
}

void serialWriteVoltageSample(int voltage) {      // 1023: 00000011 11111111
  byte byte1 = ((voltage >> NBBY) & 0b11111111);  //       00000000 00000011
  byte byte2 = (voltage & 0b11111111);            //       00000000 11111111
  Serial.write(byte1);
  Serial.write(byte2);
  Serial.write(end_of_sample_frame_byte);
//  byte bytes[3] = { byte1, byte2, end_of_sample_frame_byte };
//  Serial.write(bytes, 3);
}

void loop() {
//  int voltage_value = analogRead(analogIn);
  int current_millis = millis();
  double sample = ((current_millis % 1000) / 1000.0);  // ramp wave, period 1s
  double voltage_value_percentage = sample * double(voltage_max_value);
  int voltage_value = (int)voltage_value_percentage;

  if (current_millis % (1000 / sample_production_rate_hertz) == 0) {
    serialWriteVoltageSample(voltage_value);
  }
}
