#include <Servo.h>
#include <Wire.h>
#include <LIDARLite.h>

#define tServo A0
#define bServo A2

Servo topServo;
Servo botServo;
LIDARLite Laser;

int posTT = 80;   //Top Servo Position
int posB = 0;     //Bottom Servo Position
int posBB = 180;  //Bottom Floor Sweep End Position
int posBL = 0;    //Bottom Left Stop Position
int posBM = 95;   //Bottom Midpoint Stop Position
int posTM = 80;   //Top Midpoint Stop Position
int scan = 20;   //Number of Laser Scans
void setup() {
//  Serial.begin(9600);     //Serial Monitoring For Servo Troubleshooting
  Serial.begin(115200);   //Serial Monitoring For Lidar Troubleshooting
  mainSetup();
}

void loop() {

//  backSweep(100,posBB);
//  delay(1000);
  sweep(100, posTT,scan);
  delay(1000);
//  backfloorSweep(500, posTTB,scan);
//  delay(1000);
  floorSweep(250, posB,scan);
  delay(1000);
  sweep(100, posTT,scan);
  delay(1000);
  midStop(2000, posBM,scan);
  delay(1000);
  sweep(100, posTT,scan);
  delay(1000);
  botleftStop(1000,posBL,scan);
}

void mainSetup() {
  topServo.attach(tServo);
  botServo.attach(bServo);
  Laser.begin(0,true);
 /* Parameters
    ----------------------------------------------------------------------------
    configuration:  Default 0.
      0: Default mode, balanced performance.
      1: Short range, high speed. Uses 0x1d maximum acquisition count.
      2: Default range, higher speed short range. Turns on quick termination
          detection for faster measurements at short range (with decreased
          accuracy)
      3: Maximum range. Uses 0xff maximum acquisition count.
      4: High sensitivity detection. Overrides default valid measurement detection
          algorithm, and uses a threshold value for high sensitivity and noise.
      5: Low sensitivity detection. Overrides default valid measurement detection
          algorithm, and uses a threshold value for low sensitivity and noise.
    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      operating manual for instructions.
  */
  Laser.configure(0);
  Serial.println("Setup Complete");

}
void laserRead(int timer, int i){
  for (int j = 0; i > j;j++){
    Serial.print("Object Distance:    ");
    Serial.println(Laser.distance());
    delay(10);
  }
}
void sweep(int timer, int posTT, int scan) {
  for (posTT; posTT <= 180; posTT += 5) {
//    Serial.print("(1)The position of Top is : ");
//    Serial.print(posTT);
//    Serial.print("  Movement Function: ");
    Serial.println("Sweep Function");
    laserRead(timer,scan);
    topServo.write(posTT);
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
    botServo.write(posB);
    delay(timer);
  }
}

void midStop(int timer, int posBM, int scan) {
//  Serial.print("(5)The position of Top is : ");
//  Serial.print(posBM);
//  Serial.print("  Movement Function: ");
  Serial.println("Mid-Stop Function.!");
  laserRead(timer,scan);
  botServo.write(posBM);
  topServo.write(posTM);
  delay(timer);
}

void botleftStop(int posBL, int timer, int scan){
//  Serial.print("(6)The position of Top is : ");
//  Serial.print(posBL);
//  Serial.print("  Movement Function: ");
  Serial.println("Bottom Left Stop");
  laserRead(timer,scan);
  botServo.write(posBL);
  delay(timer);
}
/*
void backfloorSweep(int timer, int posBB, int scan) {
  for (posBB; posBB <= 180; posBB -= 5) {
    botServo.write(posBB);
    Serial.print("(4)The position of Top is : ");
    Serial.print(posBB);
    Serial.print("  Movement Function: ");
    Serial.println("Backward Top Function");
    delay(timer);
  }
}

void backSweep(int timer, int posTTB, int scan) {
  for (posTB; posTB >= 180; posTB -= 5) {
    topServo.write(posTB);
    Serial.print("(2)The position of Top is : ");
    Serial.print(posTB);
    Serial.print("  Movement Function: ");
    Serial.println("Bottom Backward Function");
    delay(timer);
  }
}
*/
