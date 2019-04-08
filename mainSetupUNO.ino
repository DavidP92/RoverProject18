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

void laserRead(){
  for (int j = 0; scan > j;j++){
    Serial.print("Object Distance:    ");
    Serial.println(Laser.distance());
    xDistance = Laser.distance();
    delay(24);
  }
}
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
      }
  }
  else if ( dir ==1){
      Serial.println("Backward Pan Motion");
        for (posPan = 140; posPan >= 0; posPan -= 12) { 
        panServo.write(posPan);  
        Serial.println(posPan);                          
      }
  }
    else if(dir==2){
      Serial.println("Sideways Pan");
      posPan = 115;     
      panServo.write(posPan);
    }
    
    else if(dir==3){
      Serial.println("Straight @ Attention");
      posPan = 10;
      panServo.write(posPan);
    }
  }
void tiltMotion(byte dir){
    if(dir == 0){
        Serial.println("Midpoint Tilt Motion");
        posTilt = 0;
        tiltServo.write(posTilt);
        Serial.println(posTilt); 
        laserRead();   
        delay(100);                  
      }
      else if(dir == 1){
        Serial.println("Backward Tilt Motion");
        posTilt = 8;
        tiltServo.write(posTilt);
        Serial.println(posTilt);            
        laserRead();
        delay(200);                   
      
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
        Serial.println();
      }
      break;
    case 4:
      Serial.println("Heading of The Griffin: ");
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

void setup(){
  Serial.begin(115200);
  panServo.attach(A0);
  tiltServo.attach(A2);
  Laser.begin(0, true);
  Laser.configure(0);
  Serial.println("Laser Setup Completed");
  if (myIMU.begin(
        MODE_SPI,
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
   panMotion(2);
   tiltMotion(0);
   sendCommand(0);
   if(xDistance <= 50){
     sendCommand(4); 
     panMotion(3);
     tiltMotion(1);
     delay(1000);
     if(xDistance < 80){
      sendCommand(3);
      delay(1000);
     }
     else{
      sendCommand(2);
      delay(1000);
     }
   }
}
