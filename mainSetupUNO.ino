#include <Wire.h>
#include <LIDARLite.h>
#include <Servo.h>

#define topServo A0
#define bottServo A2
Servo topS;
Servo botS;
LIDARLite Laser;

int xDistance = 0; 
int posTT = 80;   //Top Servo Position
int posB = 0;     //Bottom Servo Position
int posBB = 180;  //Bottom Floor Sweep End Position
int posBL = 0;    //Bottom Left Stop Position
int posBM = 95;   //Bottom Midpoint Stop Position
int posTM = 80;   //Top Midpoint Stop Position
int scan = 6;    //Number of Laser Scans

unsigned long start;      //Timer
unsigned long timer_1;    //Timer ---> Laser Function
unsigned long timer_2;  //Timer----> Motor Command

uint8_t delay_1 = 1500;
uint8_t delay_2 = 3000;

void panTiltSetup() {
  topS.attach(topServo);
  botS.attach(bottServo);
  Laser.begin(0, true);
  Laser.configure(0);
  Serial.println("Laser Setup Completed");
}
void sendCommand(byte dir) {
  Wire.beginTransmission(69);          // transmit to device #8               // sends five bytes
  Wire.write(dir);                     // sends one byte
  Wire.endTransmission();              // stop
  delay(1000);
  Serial.println(dir);
}
void setup() {
  Serial.begin(115200);
  Serial.println("Starting Setup..............");
  panTiltSetup();
 // Wire.begin();
  timer_1 = millis();
  timer_2 = timer_1;
  Serial.println("Setup has been completed....");
}
void laserRead(int timer, int i){
  for (int j = 0; i > j;j++){
    Serial.print("Object Distance:    ");
    xDistance = Laser.distance();
    Serial.println(xDistance);
    delay(100);
  }
}

void loop() {
  midStop(2000, posBM,scan);
  delay(1000);
  if(xDistance < 30){
    sendCommand(0);
  }
  else{
    sendCommand(1);
  }
}
void sweep(int timer, int posTT, int scan) {
  for (posTT; posTT <= 160; posTT += 5) {
//    Serial.print("(1)The position of Top is : ");
//    Serial.print(posTT);
//    Serial.print("  Movement Function: ");
    Serial.println("Sweep Function");
    laserRead(timer,scan);
    topS.write(posTT);
    delay(timer);
  }
}

void floorSweep(int timer, int posB, int scan) {
  for (posB; posB <= 185; posB += 5) {
//    Serial.print("(3)The position of Top is : ");
//    Serial.print(posB);
//    Serial.print("  Movement Function: ");
    Serial.println("Floor Sweep Function");
    laserRead(timer,scan);
    botS.write(posB);
    delay(timer);
  }
}

void midStop(int timer, int posBM, int scan) {
//  Serial.print("(5)The position of Top is : ");
//  Serial.print(posBM);
//  Serial.print("  Movement Function: ");
  Serial.println("Mid-Stop Function.!");
  laserRead(timer,scan);
  botS.write(posBM);
  topS.write(posTM);
  delay(timer);
}

void botleftStop(int posBL, int timer, int scan){
//  Serial.print("(6)The position of Top is : ");
//  Serial.print(posBL);
//  Serial.print("  Movement Function: ");
  Serial.println("Bottom Left Stop");
  laserRead(timer,scan);
  botS.write(posBL);
  delay(timer);
}
