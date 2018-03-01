// analog-plot
// 
// Read analog values from A0 and A1 and print them to serial port.
//
// electronut.in
 
#include "Arduino.h"
 
void setup()
{
  // initialize serial comms
  Serial.begin(460800); 
}
 
void loop()
{
  // read analog pins and save as integer
  int val1 = analogRead(0);
  int val2 = analogRead(1);
  int val3 = analogRead(2);
  int val4 = analogRead(3);
  int val5 = analogRead(4);
  int val6 = analogRead(5);
  // print to serial
  Serial.print(val1);
  Serial.print(" ");
  Serial.print(val2);
  Serial.print(" ");
  Serial.print(val3);
  Serial.print(" ");
  Serial.print(val4);
  Serial.print(" ");
  Serial.print(val5);
  Serial.print(" ");
  Serial.print(val6);
  Serial.print("\n");
  // wait 
  delay(20);
}
