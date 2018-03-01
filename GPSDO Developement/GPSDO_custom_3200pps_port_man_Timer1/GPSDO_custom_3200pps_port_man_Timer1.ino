#include <TimerOne.h>
const int sampling_clock_out_pin = 9;
void setup()
{
  pinMode(2, INPUT);  
  attachInterrupt(0, pulsePPS, RISING);//watch out for interrupt (1PPS) on pin 2
  //Timer1.initialize(400);//for 2500 pulses per sec
  Timer1.initialize(312.5);//for 2500 pulses per sec
  //Timer1.initialize(2000);//for 50 pulses per sec
  Timer1.pwm(sampling_clock_out_pin, 100);//duty cycle of the pulse, i.e. about 80uS
}

void loop()
{

}

void pulsePPS() //interrupt routine upon receiving PPS
{
  Timer1.restart();
}

