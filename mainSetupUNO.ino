#include <Wire.h>

void sendCommand(byte dir) {
  Wire.beginTransmission(69);          // transmit to device #8               // sends five bytes
  Wire.write(dir);                     // sends one byte
  Wire.endTransmission();              // stop
  delay(1000);
}
void setup() {
  Wire.begin();
}

void loop() {
  int z = 2;
  int x = 0;
  int y = 1;
  int w = 3;
  sendCommand(y);
  delay(5000);
  sendCommand(x);
  delay(5000);
  sendCommand(z);
  delay(5000);
  sendCommand(w);
  delay(5000);
}
