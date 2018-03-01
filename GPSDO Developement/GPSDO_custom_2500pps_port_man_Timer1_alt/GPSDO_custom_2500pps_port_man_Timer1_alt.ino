#include <TimerOne.h>
int sampling_clock_out_pin = 9;
void setup()
{
    // set pins 8..13 as output...
  DDRB = B00111111;  // digital pins -,-,13,12,11,10,9,8
  // set pins 1 (serial transmit) and 3..7 as output,2 as input (Interrupt PPS IN)
  // but leave pin 0 (serial receive) as input (otherwise serial port will stop working!) ...
  DDRD = B11111010;  // digital pins 7,6,5,4,3,2,1,0
  //pinMode(2, INPUT);
  Timer1.initialize(400);
  //Timer1.attachInterrupt(sampling_clock_out);
  attachInterrupt(0, pulsePPS, RISING);
}

void loop()
{
Timer1.pwm(sampling_clock_out_pin,32);
}

void pulsePPS() //interrupt routine and a single 50us pulse
{
  //ppsStart = true;
  Timer1.restart();
}

