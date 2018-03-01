/*
  Analog input, serial output

  Reads an analog input pin, prints the results to the serial monitor.

  The circuit:

  * Potentiometer connected to analog pin 15.
  * Center pin of the potentiometer goes to the analog pin.
  * Side pins of the potentiometer go to +3.3V (VCC) and ground

  created over and over again
  by Tom Igoe and everyone who's ever used Arduino

  Ported to Maple 27 May, 2010 by Bryan Newbold
*/

// Analog input pin.  You may need to change this number if your board
// can't do analog input on pin 15.
const int analogInputPin1 = 15;
const int analogInputPin2 = 16;
const int analogInputPin3 = 17;


void setup() {
  // Declare analogInputPin as INPUT_ANALOG:
  pinMode(analogInputPin1, INPUT_ANALOG);
  pinMode(analogInputPin2, INPUT_ANALOG);
  pinMode(analogInputPin3, INPUT_ANALOG);
}

void loop() {
  // Read the analog input into a variable:
  int analogValue1 = analogRead(analogInputPin1);
  int analogValue2 = analogRead(analogInputPin2);
  int analogValue3 = analogRead(analogInputPin3);
  

  // print the result:
  SerialUSB.print(analogValue1);
  SerialUSB.print(',');
  SerialUSB.print(analogValue2);
  SerialUSB.print(',');
  SerialUSB.print(analogValue3);
  SerialUSB.print('\r');
  //delayMicroseconds(33);
  
}
