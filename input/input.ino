const int analogIn = 0;
const int serialBaudRate = 9600;
const int sample_production_rate_hertz = 900;
const int voltage_max_value = 1024;
unsigned int a = 0;
const byte end_of_sample_frame_byte = 0b11111111;

void setup() {
  Serial.begin(serialBaudRate);
}

void send_frame(int *samples, size_t num_samples) {
  for (unsigned int i = 0; i < num_samples; i++) {
    send_voltage_sample(samples[i]);
  }
  Serial.write(end_of_sample_frame_byte);
}

void send_voltage_sample(int voltage) {
  // voltage : 00000010 10111111
  // byte1   :          00010101
  // byte2   :          00011111

  // assert(CHAR_BIT == 8);
  // assert(sizeof(unsigned short int) == 2);
  // assert(sizeof(unsigned char) == 1);

  unsigned char byte1 = ((voltage >> 5 ) & 0b00011111);
  unsigned char byte2 = ((unsigned short int)(voltage << 11)) >> 11;

  Serial.write(byte1);
  Serial.write(byte2);
}

void loop() {
  int input_0_sample = (a += 1) % 1024;
  int samples[1] = { input_0_sample };
  send_frame(samples, 1);
}
