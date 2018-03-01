volatile unsigned long value;
int R[2500];
int Y[2500];
int B[2500];
unsigned long int et;
void setup()
{
  //ADC setup
  ADC->ADC_WPMR &= 0xFFFE; //disable write protect
  ADC->ADC_CHER = 0b11100000; //Enable AD7,AD6,AD5 or CH7,Ch6,Ch5 or PA16,PA24,PA23 or A0,A1 and A2     |
//ADC->ADC_MR &= 0b11111111000000001111111100000000;
  ADC->ADC_MR &= 0b11111111000000000000000100000000;
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
  for (int x = 0; x < 2500; x++)
  {
    ADC->ADC_CR |= 0b10; //start conversion
    while (!(ADC->ADC_ISR & 0b11100000)); //wait for conversion to end

    for (int i = 5; i <= 7; i++)
    {
      value = ADC->ADC_CDR[i]; //read conversion data for all channels
      Serial.print(value);
      Serial.print(",");
    }
    Serial.println();
  }
  et = micros()-et;//current time - start time
  Serial.print("Time elapsed is");
  Serial.println(et);
  
  Serial.println();

  delay(10000);
}
