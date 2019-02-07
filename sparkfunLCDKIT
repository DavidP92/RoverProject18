
#include <SoftwareSerial.h>

SoftwareSerial lcd(2, 3);      // This is required, to start an instance of an LCD ( TX , RX )

void setup()
{
  lcd.begin(9600);             // Start the LCD at 9600 baud
  clearDisplay();              // Clear the display
  
  printState('still',2,200);            
  lcd.print("Hello, world!");  // Statement that prints to 1st row, 3rd column
  printState('still',17,200);            
  lcd.print("Begin Setup..."); // Statement that prints SIMULTANEOUSLY to 2nd row, 2nd column
  delay(2000);
  
  for (int i=0; i<3; i++)      // Flashes the backlight while above statements display on startup:
  {
    printState('still',0,0);
    delay(300);
    printState('still',0,255);
    delay(300);          
  }
}

void loop()
 {
    clearDisplay();            // Clear the display
    delay(500);
    printState('still',4,155);           // set cursor to 1st row, 5th column
    lcd.print("LCD TEST");
    delay(1000);
    printState('still',17,200);          // set cursor to 2nd row, 3rd column
    lcd.print("Great Success!");
    delay(3000);
    screenState('off');
  }

void clearDisplay()            // Clears out the display, MUST USE for this kit!
{
  lcd.write(0xFE);             // send the SPECIAL COMMMAND! (necessary for thes commands)
  lcd.write(0x01);             // send the clear screen command
}


void printState(char dire, byte pos, byte brightness){
  lcd.write(0x80);             
  lcd.write(brightness);
  
  if(dire == 'right'){
    lcd.write(0xFE);  
    lcd.write(0x1C);}
  else if(dire == 'left'){
    lcd.write(0xFE);  
    lcd.write(0x18);}
  else if (dire == 'still'){
    lcd.write(0xFE);  
    lcd.write(0x80);  
    lcd.write(pos);}
}

void screenState(char state){
  if(state == 'off'){
  lcd.write(0xFE);  
  lcd.write(0x08);  
}
else if(state == 'on'){
  lcd.write(0xFE);  
  lcd.write(0x18);  
}
}
