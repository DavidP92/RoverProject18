#include <Servo.h>
#include <Wire.h>
#include <LIDARLite.h>
#include "SparkFunIMU.h"
#include "SparkFunLSM303C.h"
#include "LSM303CTypes.h"

Servo panServo;
Servo tiltServo;
LIDARLite Laser;
LSM303C myIMU;

int xDistance = 0; 
int posPan = 0;    
int posTilt = 0;
int scan = 1;
float headingDegrees;

unsigned long start;      //Timer
unsigned long timer_1;    //Timer ---> Laser Function
unsigned long timer_2;  //Timer----> Motor Command
uint8_t delay_1 = 100;
uint8_t delay_2 = 3000;

void sendCommand(byte dir) {
  Wire.beginTransmission(69);          // transmit to device #8               // sends five bytes
  Wire.write(dir);                     // sends one byte
  Wire.endTransmission();              // stop
  delay(1000);
  Serial.print("The direction value being sent:   ");
  Serial.println(dir);
}

void panMotion(byte dir){
  if(dir == 0){
      Serial.println("Forward Pan Motion");
      for (posPan = 0; posPan <= 160; posPan += 12) { 
        panServo.write(posPan);
        Serial.println(posPan);   
        laserRead();                     
      }
  }
  else if ( dir ==1){
      Serial.println("Backward Pan Motion");
        for (posPan = 140; posPan >= 0; posPan -= 12) { 
        panServo.write(posPan);  
        Serial.println(posPan);           
        laserRead();                    
      }
  }
    else if(dir==2){
      posPan = 115;      panServo.write(posPan);
      laserRead();
    }
    
    else if(dir==3){
      posPan = 0;
      panServo.write(posPan);
      laserRead();
      if(xDistance < 40){
        sendCommand(4);
        sendCommand(2);
      }
      else{
        sendCommand(0);
      }
    }
  }
void tiltMotion(byte dir){
    if(dir == 0){
      Serial.println("Forward Tilt Motion");
      for (posTilt = 0; posTilt <= 25; posTilt += 5) { 
        tiltServo.write(posTilt);
        Serial.println(posTilt); 
        laserRead();   
        delay(100);                  
      }
    }
    else if(dir == 1){
      Serial.println("Backward Tilt Motion");
      for (posTilt = 25; posTilt >= 0; posTilt -= 2) { 
        tiltServo.write(posTilt);
        Serial.println(posTilt);            
        laserRead();
        delay(100);                   
      }
    }
}
void readCompass(int item)
{
  switch (item)
  {
      float value;
      float heading;
    case 0:
      value = myIMU.readAccelX();
      // Assume that if X is not activated then none are (poor assumption, but demo)
      if ( !isnan(value) )
      {
        Serial.print("\nAccelerometer:\n X = ");
        Serial.println(value, 4);
        Serial.print(" Y = ");
        Serial.println(myIMU.readAccelY(), 4);
        Serial.print(" Z = ");
        Serial.println(myIMU.readAccelZ(), 4);
      }
      break;
    case 1:
      value = myIMU.readGyroX();
      // Not supported by hardware, so will return NAN
      if ( !isnan(value) )
      {
        Serial.print(F("\nGyroscope:\n X = "));
        Serial.println(value, 4);
        Serial.print(" Y = ");
        Serial.println(myIMU.readGyroY(), 4);
        Serial.print(" Z = ");
        Serial.println(myIMU.readGyroZ(), 4);
      }
      break;
    case 2:
      value = myIMU.readMagX();
      if ( !isnan(value) )
      {
        Serial.print("\nMagnetometer:\n X = ");
        Serial.println(value, 4);
        Serial.print(" Y = ");
        Serial.println(myIMU.readMagY(), 4);
        Serial.print(" Z = ");
        Serial.println(myIMU.readMagZ(), 4);
      }
      break;
    case 3:
      value = myIMU.readTempC();
      if ( !isnan(value) )
      {
        Serial.print("\nThermometer:\n");
        Serial.print(" Degrees C = ");
        Serial.println(value, 4);
        Serial.print(" Degrees F = ");
        Serial.println(myIMU.readTempF(), 4);
      }
      break;
    case 4:
      heading = atan2(myIMU.readMagY(), myIMU.readMagX());
      if (heading < 0)
        heading += 2 * PI;
      headingDegrees = heading * 180 / M_PI;
      Serial.print("Theta: ");
      Serial.println(headingDegrees);
      break;
    default:
      Serial.println(F("Failed to enter in a vaild option."));
  }
}
void laserRead(){
  for (int j = 0; scan > j;j++){
    Serial.print("Object Distance:    ");
    Serial.println(Laser.distance());
    xDistance = Laser.distance();
    delay(24);
  }
}
void setup(){
  Serial.begin(115200);
  panServo.attach(A0);
  tiltServo.attach(A2);
  Laser.begin(0, true);
  Laser.configure(0);
  Serial.println("Laser Setup Completed");
  if (myIMU.begin(
        MODE_I2C,
        MAG_DO_40_Hz,
        MAG_FS_16_Ga,
        MAG_BDU_ENABLE,
        MAG_OMXY_HIGH_PERFORMANCE,
        MAG_OMZ_HIGH_PERFORMANCE,
        MAG_MD_CONTINUOUS,
        ACC_FS_2g,
        ACC_BDU_ENABLE,
        ACC_X_ENABLE | ACC_Y_ENABLE | ACC_Z_ENABLE,
        ACC_ODR_100_Hz
      ) != IMU_SUCCESS)
  {
    Serial.println(F("Compass/Magnet/Accel Failed setting up."));
    while (1);
  }
  Serial.println("Setup Configurations is completed.......");
  timer_1 = millis();
}

void loop() {
   int i = 0;
   start = millis();
   panMotion(3);
   tiltMotion(0);
   if(xDistance < 50){
    sendCommand(4);
    sendCommand(2);
    i = i + 1;
    if(xDistance < 40){
      sendCommand(3);
      sendCommand(3);
      i = i + 1;
    }
    else{
      sendCommand(0);
    }
      if (xDistance < 40){
        sendCommand(3); 
        }
   }
}
