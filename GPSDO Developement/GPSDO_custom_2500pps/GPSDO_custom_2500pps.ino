int pinout = 9;
boolean ppsStart = false;


#include <eRCaGuy_Timer2_Counter.h>
unsigned long t_start;
unsigned long t_T2_count;

void setup()
{
  timer2.setup();
  pinMode(pinout, OUTPUT);
  pinMode(2, INPUT);
  attachInterrupt(0, pulsePPS, RISING);
}

void loop()
{
  if(ppsStart == true) {
    ppsStart = false;
    //timer2_Microseconds_delay(35); //A necesary ofset I need to be able to adjust
    for (unsigned long i = 0; i < 2499 ; i++) {
      digitalWrite(pinout, HIGH);
      timer2_Microseconds_delay(50); //50us wave
      digitalWrite(pinout, LOW);
      //delayMicroseconds(1931); // tweak for correct pulse spacing
      //timer2_Microseconds_delay(350);
      timer2_Microseconds_delay(275);
    }
    digitalWrite(pinout, HIGH);
    timer2_Microseconds_delay(50); //50us wave
    digitalWrite(pinout, LOW);
  }
}
void timer2_Microseconds_delay(unsigned long delay_micros){
  t_start = timer2.get_micros(); //units of 0.5us; the count accumulated by Timer2_Counter
  t_T2_count = timer2.get_micros();
  while((t_T2_count - t_start) < delay_micros){
  //Do Nothing (WAIT)
  t_T2_count = timer2.get_micros();
  }
}

void pulsePPS() //interrupt routine and a single 50us pulse
{
  ppsStart = true;
}

