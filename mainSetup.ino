
#include <XBee.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "SparkFunIMU.h"
#include "SparkFunLSM303C.h"
#include "LSM303CTypes.h"

#define slave 0x01

SoftwareSerial outputSerial(10, 9); // RX, TX
LSM303C roverIMU;

XBee xbee = XBee();
Rx16Response rx16 = Rx16Response();
int resetRSSI = -1000;    //The value that RSSI is reset to after each pass through filter
#define samples 110
int temp, smoothData, rawData;
int timeToScan = 2000;
short currentHeading;
float headingDegrees;
float beaconHeading; 

//Variable for i2c comms
uint8_t currHeadingI2c[2];

//Structure to contain the readings from the beacon
struct{
  float heading;
  int signalStrength;
} readings[samples];

//Union for converting between byte[4] and float
union{
  float f;
  uint8_t b[4];
} heading_converter;

void setup() {
  beaconHeading = 0;
  
  Serial.begin(57600); 
 // Serial1.begin(57600);
//  xbee.setSerial(Serial1);
  outputSerial.begin(57600);

  //Initialize i2c communications
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(i2cPrint); // register event
}

void loop() {
  int start = millis();
  
  //Process the data, print the result, and reset.
  currentHeading = (ProcessData());
  currHeadingI2c[0] = 0xFF&(currentHeading>>8);
  currHeadingI2c[1] = 0xFF&currentHeading;
}


/////////////////////////////////////////////////////
////////////////Local Functions//////////////////////
/////////////////////////////////////////////////////
void compassFunction(int item)
{
  switch (item)
  {   float value;
      float heading;
    case 0:
      value = roverIMU.readAccelX();
      // Assume that if X is not activated then none are (poor assumption, but demo)
      if ( !isnan(value) )
      {
        Serial.print("\nAccelerometer:\n X = ");
        Serial.println(value, 4);
        Serial.print(" Y = ");
        Serial.println(roverIMU.readAccelY(), 4);
        Serial.print(" Z = ");
        Serial.println(roverIMU.readAccelZ(), 4);
      }
      break;
    case 1:
      value = roverIMU.readGyroX();
      // Not supported by hardware, so will return NAN
      if ( !isnan(value) )
      {
        Serial.print(F("\nGyroscope:\n X = "));
        Serial.println(value, 4);
        Serial.print(" Y = ");
        Serial.println(roverIMU.readGyroY(), 4);
        Serial.print(" Z = ");
        Serial.println(roverIMU.readGyroZ(), 4);
      }
      break;
    case 2:
      value = roverIMU.readMagX();
      if ( !isnan(value) )
      {
        Serial.print("\nMagnetometer:\n X = ");
        Serial.println(value, 4);
        Serial.print(" Y = ");
        Serial.println(roverIMU.readMagY(), 4);
        Serial.print(" Z = ");
        Serial.println(roverIMU.readMagZ(), 4);
      }
      break;
    case 3:
      value = roverIMU.readTempC();
      if ( !isnan(value) )
      {
        Serial.print("\nThermometer:\n");
        Serial.print(" Degrees C = ");
        Serial.println(value, 4);
        Serial.print(" Degrees F = ");
        Serial.println(roverIMU.readTempF(), 4);
      }
      break;
    case 4:
      heading = atan2(roverIMU.readMagY(), roverIMU.readMagX());
      if (heading < 0)
        heading += 2 * PI;
      headingDegrees = heading * 180 / M_PI;
      Serial.print("Theta: ");
      Serial.println(headingDegrees);
      break;
    default:
      Serial.println(F("Failed to enter in a vaild option."));
  }

  if (millis() % 5200 == 0)
    processSignal();
}

void Retrieve(int i){
  xbee.readPacket(10);    //Wait 50 to receive packet
  if (xbee.getResponse().isAvailable())     //Execute only if packet found
  {
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) 
    {
      xbee.getResponse().getRx16Response(rx16);
      //Store the transmitted data and RSSI
      for(int i = 0; i<4; i++) heading_converter.b[i] = rx16.getData(i);
      int currentRSSI = -rx16.getRssi();

      //Write to array
      readings[i].heading = heading_converter.f;
      readings[i].signalStrength = currentRSSI;
    }
  }else{
    readings[i].heading = 0;
    readings[i].signalStrength = resetRSSI;
  }
}

          


//Creates a heading through averaging the readings
int ProcessData(){
  int maxRSSI;
  unsigned long maxIndex = 0;
  maxRSSI = readings[0].signalStrength;
  
  //Find max RSSI value
  for (int i=1; i < samples; i++) {
    if (maxRSSI < readings[i].signalStrength) {
      maxRSSI = readings[i].signalStrength;
      maxIndex = i;
    }
  }
  //If there is no valid data
  if(maxRSSI == resetRSSI){
    return -1;
  }

  float headingx = 0;
  float headingy = 0;
  for(int i = 0; i < samples; i++)
  {
    if (readings[i].signalStrength == -1000 && readings[i].heading == 0)
    {
       Serial.println("this heading not included");
    }
    else
    {
      Serial.print(readings[i].heading);
      Serial.print("\t");
      Serial.println(readings[i].signalStrength);
      // Set magnitude of vector by signal strength
      headingx += readings[i].signalStrength * cos(readings[i].heading * PI / 180);
      headingy += readings[i].signalStrength * sin(readings[i].heading * PI / 180);
    }
  }
  
  float heading = atan2(headingy, headingx);
  if (heading < 0) heading += 2 * PI;
  heading = heading * 180 / PI;

  return (int) heading;
}

//When info is requested over i2c
void i2cPrint()
{
  Wire.write(currHeadingI2c, 2);
  // as expected by master
}

void processSignal(){
for(int i = 0;i<samples;i++){
    Retrieve(i);
    float propComplete = ((float)i)/(float)samples;
    delay(timeToScan/samples);
  }
  beaconHeading = (ProcessData());
  Serial.println(currentHeading);
  outputSerial.println(currentHeading);
}
void contactSlave(int data)
{
    Wire.beginTransmission(slave);
    Wire.write(data);
    Wire.endTransmission();
}
