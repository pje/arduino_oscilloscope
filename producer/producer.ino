const int analogIn = A0;
const int serialBaudRate = 28800;
const int voltage_max_value = 1024;
const byte end_of_sample_frame_byte = 0xff; // 0b11111111

void setup(void);
void send_frame(int *samples, size_t num_samples);
void send_voltage_sample(int voltage);
void loop(void);

void setup(void) {
  Serial.begin(serialBaudRate);
}

void send_frame(int *samples, size_t num_samples) {
  for (unsigned int i = 0; i < num_samples; i++) {
    send_voltage_sample(samples[i]);
  }
  Serial.write(end_of_sample_frame_byte);
}

void send_voltage_sample(int voltage) {
  // assert(CHAR_BIT == 8);
  // assert(sizeof(unsigned short int) == 2);
  // assert(sizeof(unsigned char) == 1);
  //
  // voltage : 00000010 10111111
  // byte1   :          00010101
  // byte2   :          00011111

  unsigned char byte1 = ((voltage >> 5 ) & 0x1f); // 0b00011111
  unsigned char byte2 = ((unsigned short int)(voltage << 11)) >> 11;

  Serial.write(byte1);
  Serial.write(byte2);
}

void loop(void) {
  int input_0_sample = analogRead(analogIn);
  int samples[1] = { input_0_sample };
  send_frame(samples, 1);
}
