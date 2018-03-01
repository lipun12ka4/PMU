int pinout = 9;
unsigned long time = 0;
int pulseCount = 0;
boolean ppsStart = false;
boolean timing = false;

//#include <eRCaGuy_Timer2_Counter.h>

void setup()
{
  //timer2.setup();
  pinMode(pinout, OUTPUT);
  pinMode(2, INPUT);
  attachInterrupt(0, pulsePPS, RISING);
}

void loop()
{
  if(ppsStart == true) {
    ppsStart = false;
    delayMicroseconds(35); //A necesary ofset I need to be able to adjust
    for (int i = 0; i < 2499 ; i++) {
      digitalWrite(pinout, HIGH);
      //timer2_Microseconds_delay(50); //50us wave
      delayMicroseconds(50);
      digitalWrite(pinout, LOW);
      //delayMicroseconds(1931); // tweak for correct pulse spacing
      //timer2_Microseconds_delay(350);
      delayMicroseconds(350);
    }
    digitalWrite(pinout, HIGH);
    //timer2_Microseconds_delay(50); //50us wave
    delayMicroseconds(50);
    digitalWrite(pinout, LOW);
  }
}
//void timer2_Microseconds_delay(unsigned long delay_micros){
//  static unsigned long t_start = timer2.get_count(); //units of 0.5us; the count accumulated by Timer2_Counter
//  unsigned long t_T2_count = timer2.get_count();
//  while(((t_T2_count-t_start)/2)<delay_micros){
//  //Do Nothing (WAIT)
//  t_T2_count = timer2.get_count();
//  }
//}

void pulsePPS() //interrupt routine and a single 50us pulse
{
  ppsStart = true;
}

