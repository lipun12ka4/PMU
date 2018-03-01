/*

 Analog Read (Pin 0) Speed Test
 
 Created 2 September 2010, last updated 03 September 2010
 By leaflabs.com forums member "Stephen from NYC"
 
 This code is released under a Creative Commons Attribution-Share Alike 3.0 license.
 
 Maple Rev 3 (IDE 0.0.6): STM32 Cortex M3
 
 1000 reads: 7 milliseconds
 10000 reads: 68 milliseconds
 100000 reads: 683 milliseconds
 1000000 reads: 6825 milliseconds
 
 Calculations: 6.825 microseconds per analogRead() for the Maple Rev 3 [146,520 analogReads per second]
 
 Arduino Duemilanove 2009 (IDE 017): ATMega 328
 
 1000 reads: 112 milliseconds
 10000 reads: 1120 milliseconds
 100000 reads: 11200 milliseconds
 
 Calculations: 112 microseconds per analogRead() for the Arduino Duemilanove [8,928 analogReads per second]
 
 Conclusion: Using this simple benchmark the Maple is 16.4X faster than the Arduino. A signficant increase, but not as large as one would expect from the specifications.
 
 */

#define COMM SerialUSB // use this for Maple
// #define COMM Serial // use this for Arduino

unsigned long start=0;
unsigned long stop=0;

unsigned long counter=0;

unsigned long limit=1000;

void setup()
{
  // the following line is needed for Maple
  pinMode(15, INPUT_ANALOG);

  // the following line is needed for Arduino
  // COMM.begin(57600);
}

void loop()
{
  COMM.println("\nStarting loops:");

  start=millis();

  counter=0;

  while (counter++ < limit)
  {
    analogRead(0);
  }

  stop=millis();

  COMM.println("Stop loops:");
  COMM.print("Elapsed Time: ");
  COMM.print(stop-start);
  COMM.print(" milliseconds (for ");
  COMM.print(limit);
  COMM.println(" analog reads)");
}

