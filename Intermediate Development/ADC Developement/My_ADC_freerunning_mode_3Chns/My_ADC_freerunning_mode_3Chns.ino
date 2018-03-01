int signal1[10000];
int signal2[10000];
int signal3[10000];

void setup() {
  Serial.begin(115200);
  int t=analogRead(0);//for debugging

  ADC->ADC_MR |= 0x80;  // these lines set free running mode on adc
  ADC->ADC_CR=2;        // Enable 3 channels
  ADC->ADC_CHER=0xE0;   //i.e. enable A0,A1,A2 // 11100000 i.e. E0
                        // ch7:A0 ch6:A1 ch5:A2 ch4:A3 ch3:A4 ch2:A5 ch1:A6 ch0:A7
}

void loop() {
  int t=micros();
  //int q0=0,q1=0,q2=0;
  int a0,a1,a2;
  for(int i=0;i<2500;i++){
    //while((ADC->ADC_ISR & 0xC0)!=0xC0);// wait for two conversions (pin A0[7]  and A1[6])
    while((ADC->ADC_ISR & 0xE0)!=0xE0);//Wait for 3 conversions to finish
    a0=ADC->ADC_CDR[7];              // read data
    a1=ADC->ADC_CDR[6];
    a2=ADC->ADC_CDR[5];
//    q0+=a0;
//    q1+=a1;
//    q2+=a2;
//    signal1[i]=a0;
//    signal2[i]=a1;
//    signal3[i]=a2;
  }
  t=micros()-t;
  Serial.print("1 Hundred conversions in ");Serial.print(t);Serial.println(" micros");
}
