int pinout = 9;
unsigned long time = 0;
int pulseCount = 0;
boolean ppsStart = false;
boolean timing = false;

void setup()
{
  pinMode(pinout, OUTPUT);
  pinMode(2, INPUT);
  attachInterrupt(0, pulsePPS, RISING);
}

void loop()
{

  if(ppsStart == true) {
    ppsStart = false;
    delayMicroseconds(35); //A necesary ofset I need to be able to adjust
    for (int i = 0; i < 49 ; i++) {
      digitalWrite(pinout, HIGH);
      delayMicroseconds(50); //50us wave
      digitalWrite(pinout, LOW);
      //delayMicroseconds(1931); // tweak for correct pulse spacing
      delay(20);
    }
    digitalWrite(pinout, HIGH);
    delayMicroseconds(50); //50us wave
    digitalWrite(pinout, LOW);
  }


}

void pulsePPS() //interrupt routine and a single 50us pulse
{
  ppsStart = true;
}

