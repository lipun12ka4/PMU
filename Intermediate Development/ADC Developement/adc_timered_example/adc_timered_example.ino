const int ADC_FREQ = 100000;
void setup()
{
  Serial.begin(115200);
  adc_setup () ;         // setup ADC

  pmc_enable_periph_clk (TC_INTERFACE_ID + 0 * 3 + 0) ; // clock the TC0 channel 0

  TcChannel * t = &(TC0->TC_CHANNEL)[0] ;    // pointer to TC0 registers for its channel 0
  t->TC_CCR = TC_CCR_CLKDIS ;  // disable internal clocking while setup regs
  t->TC_IDR = 0xFFFFFFFF ;     // disable interrupts
  t->TC_SR ;                   // read int status reg to clear pending
  t->TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 |   // use TCLK1 (prescale by 2, = 42MHz)
              TC_CMR_WAVE |                  // waveform mode
              TC_CMR_WAVSEL_UP_RC |          // count-up PWM using RC as threshold
              TC_CMR_EEVT_XC0 |     // Set external events from XC0 (this setup TIOB as output)
              TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_CLEAR |
              TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_CLEAR ;

  t->TC_RC =  VARIANT_MCK / 2 / ADC_FREQ; // counter resets on RC, so sets period in terms of 42MHz clock
  t->TC_RA =  VARIANT_MCK / 2 / ADC_FREQ / 2 ; // roughly square wave
  t->TC_CMR = (t->TC_CMR & 0xFFF0FFFF) | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET ;  // set clear and set from RA and RC compares

  t->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG ;  // re-enable local clocking and switch to hardware trigger source.

}

void adc_setup ()
{
  NVIC_EnableIRQ (ADC_IRQn) ;   // enable ADC interrupt vector
  ADC->ADC_IDR = 0xFFFFFFFF ;   // disable interrupts
  ADC->ADC_IER = 0x80 ;         // enable AD7 End-Of-Conv interrupt (Arduino pin A0)
  ADC->ADC_CHDR = 0xFFFF ;      // disable all channels
  ADC->ADC_CHER = 0xFF ;        // ch7:A0 ch6:A1 ch5:A2 ch4:A3 ch3:A4 ch2:A5 ch1:A6 ch0:A7
  ADC->ADC_CGR = 0x15555555 ;   // All gains set to x1
  ADC->ADC_COR = 0x00000000 ;   // All offsets off

  ADC->ADC_MR = (ADC->ADC_MR & 0xFFFFFFF0) | (1 << 1) | ADC_MR_TRGEN ;  // 1 = trig source TIO from TC0
}


volatile int isr_count = 0 ;   // this was for debugging
volatile unsigned int last;


#ifdef __cplusplus
extern "C"
{
#endif

void ADC_Handler (void)
{
  //wait untill all 8 ADCs have finished thier converstion.
  while (!(((ADC->ADC_ISR & ADC_ISR_EOC7) && (ADC->ADC_ISR & ADC_ISR_EOC6) && (ADC->ADC_ISR & ADC_ISR_EOC5) && (ADC->ADC_ISR & ADC_ISR_EOC4) && (ADC->ADC_ISR & ADC_ISR_EOC3) && (ADC->ADC_ISR & ADC_ISR_EOC2) && (ADC->ADC_ISR & ADC_ISR_EOC1) && (ADC->ADC_ISR & ADC_ISR_EOC0))));

  //add all of adc values up and bit shit right three times to devide by 8
  //typcast to unsigned int so leading bits are shifted in as 0s
  int avj = ((unsigned int)(*(ADC->ADC_CDR + 7) + * (ADC->ADC_CDR + 6) + * (ADC->ADC_CDR + 5) + * (ADC->ADC_CDR + 4) + * (ADC->ADC_CDR + 3) + * (ADC->ADC_CDR + 2) + * (ADC->ADC_CDR + 1) + * (ADC->ADC_CDR + 0)) >> 3);
             //Serial.println("  avj  "+ String(avj));
             isr_count ++;
}

#ifdef __cplusplus
}
#endif


void loop()
{
}



