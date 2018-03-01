#include <TimerOne.h>
boolean ppsStart = false;
#include <eRCaGuy_Timer2_Counter.h>
unsigned long t_start;
unsigned long t_T2_count;
void setup()
{
    // set pins 8..13 as output...
  DDRB = B00111111;  // digital pins -,-,13,12,11,10,9,8
  // set pins 1 (serial transmit) and 3..7 as output,2 as input (Interrupt PPS IN)
  // but leave pin 0 (serial receive) as input (otherwise serial port will stop working!) ...
  DDRD = B11111010;  // digital pins 7,6,5,4,3,2,1,0
  //pinMode(2, INPUT);
  Timer1.initialize(400);
  Timer1.attachInterrupt(sampling_clock_out);
  attachInterrupt(0, pulsePPS, RISING);
}

void loop()
{
//  if(ppsStart == true) {
//    ppsStart = false;
//    //timer2_Microseconds_delay(35); //A necesary ofset I need to be able to adjust
//    //for (unsigned long i = 0; i < 2499 ; i++) {
//      
//      //digitalWrite(pinout, HIGH);
//      PORTB = B00000010;
//      timer2_Microseconds_delay(50); //50us wave
//      // Write simultaneously to pins 8..13...
//      // PORTB = B00111000;   // turns on 13,12,11; turns off 10,9,8
//      PORTB = B00000000;
//      //digitalWrite(pinout, LOW);
//      //delayMicroseconds(1931); // tweak for correct pulse spacing
//      //timer2_Microseconds_delay(350);
//      timer2_Microseconds_delay(275);
//    }
//    //digitalWrite(pinout, HIGH);
//    PORTB = B00000010;
//    timer2_Microseconds_delay(50); //50us wave
//    //digitalWrite(pinout, LOW);
//    PORTB = B00000000;
//  }
}
void sampling_clock_out(){
  PORTB = B00000010;
  timer2_Microseconds_delay(10); //50us wave
  PORTB = B00000000;
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
  //ppsStart = true;
  Timer1.restart();
}

