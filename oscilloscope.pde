import processing.serial.*;

Serial port;
int[] valuesA;
float zoom = 1.0f;
int headA = 0;
final int maxVoltage = 1023;
final int initialWindowWidth = 240;
final int initialWindowHeight = 240;
final int serialBaudRate = 28800;

void setup() {
  size(initialWindowWidth, initialWindowHeight);
  port = new Serial(this, Serial.list()[9], serialBaudRate);
  valuesA = new int[width + 1];
  smooth();
}

int getValue(int index) {
  return(valuesA[(headA + index) % valuesA.length]);
}

void pushValue(int value) {
  valuesA[headA] = value;
  headA = ((headA + 1) % valuesA.length);
}

int voltageToYCoordinate(int val) {
  float yPercentage = (val / ((float) maxVoltage));
  float y = height - (yPercentage * height);
  return((int)y);
}

int readValue() {
  long read = 0;
  int value = -1;
  while (port.available() >= 3) {
    if (port.read() == 0xff) {
      int byte1 = port.read();
      int byte2 = port.read();
      value = (byte1 << 8) | byte2;
    }
  }
  return value;
}

void keyReleased() {
  switch (key) {
    case '+':
      zoom *= 2.0f;
      if ( (int) (width / zoom) <= 1 ) {
        zoom /= 2.0f;
        drawGrid();
      }
      break;
    case '-':
      zoom /= 2.0f;
      if (zoom < 1.0f) {
        zoom *= 2.0f;
        drawGrid();
      }
      break;
  }
}

color base03  = color(  0,  43,  54); // brblack
color base02  = color(  7,  54,  66); // black
color base01  = color( 88, 110, 117); // brgreen
color base00  = color(101, 123, 131); // bryellow
color base0   = color(131, 148, 150); // brblue
color base1   = color(147, 161, 161); // brcyan
color base2   = color(238, 232, 213); // white
color base3   = color(253, 246, 227); // brwhite
color yellow  = color(181, 137,   0); // yellow
color orange  = color(203,  75,  22); // brred
color red     = color(220,  50,  47); // red
color magenta = color(211,  54, 130); // magenta
color violet  = color(108, 113, 196); // brmagenta
color blue    = color( 38, 139, 210); // blue
color cyan    = color( 42, 161, 152); // cyan
color green   = color(133, 153,   0); // green

void redrawLines() {
  int graphWidth = (int) (width / zoom);
  int k = valuesA.length - graphWidth;
  int x0 = 0;
  int y0 = 0;

  stroke(orange);
  k = 0;
  x0 = 0;
  y0 = voltageToYCoordinate(valuesA[k]);
  for (int i=1; i<graphWidth; i++) {
    k++;
    int x1 = (int) (i * (width-1) / (graphWidth-1));
    int y1 = (voltageToYCoordinate(getValue(k)) - 1);
    line(x0, y0, x1, y1);
    x0 = x1;
    y0 = y1;
  }
}

void drawGrid() {
  stroke(base1);
  textSize(9);
  int y_subdivision_0 = ((height / 5) * 0);
  int y_subdivision_1 = ((height / 5) * 1);
  int y_subdivision_2 = ((height / 5) * 2);
  int y_subdivision_3 = ((height / 5) * 3);
  int y_subdivision_4 = ((height / 5) * 4);
  int y_subdivision_5 = ((height / 5) * 5);
  line(0, y_subdivision_1, width, y_subdivision_1);
  line(0, y_subdivision_2, width, y_subdivision_2);
  line(0, y_subdivision_3, width, y_subdivision_3);  
  line(0, y_subdivision_4, width, y_subdivision_4);
  text("5", 3, y_subdivision_0 + 10);
  text("4", 3, y_subdivision_1);
  text("3", 3, y_subdivision_2);
  text("2", 3, y_subdivision_3);  
  text("1", 3, y_subdivision_4);
  text("0", 3, y_subdivision_5);
}

void draw() {
  background(base03);
  drawGrid();
  int val = readValue();
  if (val >= 0) {
    pushValue(val);
  }
  redrawLines();
}

