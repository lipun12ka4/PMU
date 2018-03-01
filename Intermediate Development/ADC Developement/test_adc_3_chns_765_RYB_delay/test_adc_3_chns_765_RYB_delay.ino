volatile unsigned long value;
volatile int R[2500];
volatile int Y[2500];
volatile int B[2500];
int x=0;
unsigned long int et;
void setup()
{
  //ADC setup
  ADC->ADC_WPMR &= 0xFFFE; //disable write protect
  ADC->ADC_CHER = 0b11100000; //Enable AD7,AD6,AD5 or CH7,Ch6,Ch5 or PA16,PA24,PA23 or A0,A1 and A2     |
  //ADC->ADC_MR &= 0b11111111000000001111111100000000;// Default prescalar
  //ADC->ADC_MR &= 0b11111111000000000000000100000000; //Very Fast
  ADC->ADC_MR &= 0b11111111000000000000011100000000;//Fast i.e. about 4mS for 2500 Conversions on three channels
  ADC->ADC_MR |= 0b00000000000100100000000000000000; //software trigger, hi res, no sleep, not free running
  ADC->ADC_IER = 0b11100000; //enable interrupt
  ADC->ADC_IMR = 0b11100000; //enable interrupt in mask

  ADC->ADC_CR |= 0b10; //start first conversion
  Serial.begin(115200);
}
// the loop routine runs over and over again forever:
void loop()
{
  et = micros();
  for(x=0;x<2500;x++){  
    ADC->ADC_CR |= 0b10; //start conversion
    while (!(ADC->ADC_ISR & 0b11100000)); //wait for conversion to end
    R[x] = ADC->ADC_CDR[7];
    Y[x] = ADC->ADC_CDR[6];
    B[x] = ADC->ADC_CDR[5];
    delayMicroseconds(400);
  }   
  et = micros() - et; //current time - start time
  Serial.print("Time elapsed for acquisition is");
  Serial.println(et);

  for (int i = 0; i < 2500; i++)
  {
    Serial.print(R[i]);
    Serial.print(",");
  }
  Serial.println();
  for (int i = 0; i < 2500; i++)
  {
    Serial.print(Y[i]);
    Serial.print(",");
  }
  Serial.println();
  for (int i = 0; i < 2500; i++)
  {
    Serial.print(B[i]);
    Serial.print(",");
  }
  Serial.println();

}
