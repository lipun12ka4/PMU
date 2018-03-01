int x = 0;
unsigned long int et;
void setup()
{
  Serial.begin(115200);
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
 attachInterrupt(22, aquire, RISING);
}
// Circular buffer, power of two.
#define BUFSIZE 0x32 //50 samples buffer
#define BUFMASK 0x31
volatile int R [BUFSIZE] ;
volatile int Y [BUFSIZE] ;
volatile int B [BUFSIZE] ;
volatile int sptr = 0 ;

void aquire() {
  ADC->ADC_CR |= 0b10; //start conversion
  while (!(ADC->ADC_ISR & 0b11100000)); //wait for conversion to end
  int Rval=ADC->ADC_CDR[7];
  int Yval=ADC->ADC_CDR[6];
  int Bval=ADC->ADC_CDR[5];
  
  R[sptr] = Rval;
  Y[sptr] = Yval;
  B[sptr] = Bval;
  sptr=(sptr+1) & BUFMASK;
}
// the loop routine runs over and over again forever:
void loop()
{
  int Rsamples[50];
  int Ysamples[50];
  int Bsamples[50];
  for(int i=0;i<50;i++)
  {
    Rsamples[i]=R[i];
    Ysamples[i]=Y[i];
    Bsamples[i]=B[i];
  }

  for (int i = 0; i < 50; i++)
  {
    Serial.print(Rsamples[i]);
    Serial.print(",");
  }
  Serial.println();
  for (int i = 0; i < 50; i++)
  {
    Serial.print(Ysamples[i]);
    Serial.print(",");
  }
  Serial.println();
  for (int i = 0; i < 50; i++)
  {
    Serial.print(Bsamples[i]);
    Serial.print(",");
  }
  Serial.println();
  Serial.println();


}
