//Phasoer Estimation Variable Declaration
int N = 50; //Sampling frequency 2500 Hz
long double pi = 3.143;

int adc_out_1[50];
int adc_out_2[50];
int adc_out_3[50];

int count_samples=0;

void setup()
{
  Serial.begin(460800);
  Serial.println("Ready");
  analogReadResolution(12);
  attachInterrupt(22, take_samples_trig, RISING);//sampling clock from GPSDO
}

void loop()
{
  if (count_samples == 50)
  {
     count_samples=0;
     for(int x=0;x<50;x++)
     {
       Serial.print(adc_out_1[x]);
       Serial.print(",");
     }
     Serial.println("");
  }
  }


void take_samples_trig()
{
  int a0=analogRead(A0);
  int a1=analogRead(A1);
  int a2=analogRead(A2);
  
  for (int i = N; i > 0; i--)
  {
    adc_out_1[i] = adc_out_1[i-1];
    adc_out_2[i] = adc_out_2[i-1];
    adc_out_3[i] = adc_out_3[i-1];
   }
   adc_out_1[0]=a0;
   adc_out_2[0]=a1;
   adc_out_3[0]=a2; 
  count_samples=count_samples+1; 
}


