volatile unsigned long value;

void setup() 
{
  //ADC setup
  ADC->ADC_WPMR &= 0xFFFE; //disable write protect
  ADC->ADC_CHER = 0b100; //Enable AD2     |
  ADC->ADC_MR &= 0b11111111000000001111111100000000;
  ADC->ADC_MR |= 0b00000000000100100000000000000000; //software trigger, hi res, no sleep, not free running
  ADC->ADC_IER = 0b100; //enable interrupt
  ADC->ADC_IMR = 0b100; //enable interrupt in mask

  ADC->ADC_CR |= 0b10; //start first conversion
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() 
{
  ADC->ADC_CR |= 0b10; //start conversion
  while (!(ADC->ADC_ISR & 0b100)); //wait for conversion to end 

  for (int i = 0; i <= 15; i++)
 { 
    value = ADC->ADC_CDR[i]; //read conversion data for all channels
    Serial.println(value);    
 }

 Serial.println();
 Serial.println();
 Serial.println();

 delay(3000);
}
