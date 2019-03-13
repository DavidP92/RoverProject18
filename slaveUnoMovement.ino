#include <Wire.h>

#define MOTOR_A 0
#define MOTOR_B 1
#define DIRA 2 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B
#define FORWARD  0
#define REVERSE 1

byte caughtEvent = 0;

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}
void forwardDrive(){
    driveArdumoto(MOTOR_A, FORWARD, 255); 
    driveArdumoto(MOTOR_B, FORWARD, 255); 
    Serial.println("Forward");
}
void reverseDrive(){
    driveArdumoto(MOTOR_A, REVERSE, 255); 
    driveArdumoto(MOTOR_B, REVERSE, 255); 
    Serial.println("Reverse");
}
void leftDrive(){
    driveArdumoto(MOTOR_A, FORWARD, 255); 
    driveArdumoto(MOTOR_B, REVERSE, 255); 
    Serial.println("Left");
}
void rightDrive(){
    driveArdumoto(MOTOR_A, REVERSE, 255); 
    driveArdumoto(MOTOR_B, FORWARD, 255); 
    Serial.println("Right");
}
// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

// setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
void catchEvent(int bytes){
  
  caughtEvent = Wire.read();

}
void setup() {
  Wire.begin(69);
  Serial.begin(115200);
  Wire.onReceive(catchEvent);
  setupArdumoto();
}

void loop() {
  Serial.println(caughtEvent);
  delay(1000);
  if(caughtEvent == 1){
    forwardDrive();
  }
  else if(caughtEvent == 0){
    reverseDrive();
  }
  else if(caughtEvent == 2){
    rightDrive();
  }
  else if(caughtEvent == 3){
    leftDrive();
  }
  else{
    Serial.println("Waiting for Response");
}
}
