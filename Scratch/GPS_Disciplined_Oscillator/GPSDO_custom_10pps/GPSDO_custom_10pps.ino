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
    for (int i = 0; i < 9 ; i++) {
      digitalWrite(pinout, HIGH);
      delay(2); //2 ms width pulse
      digitalWrite(pinout, LOW);
      delay(98);
    }
      digitalWrite(pinout, HIGH);
      delay(2); //2 ms width pulse
      digitalWrite(pinout, LOW);
    ppsStart = false;
  }
}

void pulsePPS() //interrupt routine and a single 50us pulse
{
  ppsStart = true;
}

