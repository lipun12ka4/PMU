int N=500;//Sampling frequency 25000 Hz
long double pi=3.143;

long double adc_out_1[500];
long double adc_out_2[500];
long double adc_out_3[500];

long double values_1[500];
long double values_2[500];
long double values_3[500];


long double Xi_1, Xr_1, Phasor_Magnitude_1, Phasor_Angle_1, Phasor_Angle_Degree_1;
long double Xi_2, Xr_2, Phasor_Magnitude_2, Phasor_Angle_2, Phasor_Angle_Degree_2;
long double Xi_3, Xr_3, Phasor_Magnitude_3, Phasor_Angle_3, Phasor_Angle_Degree_3;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  //Acquire the 500 Samples for 1 Cycle at 25000 Hz
  for (int i = 0; i < N; i++)
    {
     adc_out_1[i] = analogRead(A0);
     adc_out_2[i] = analogRead(A1);
     adc_out_3[i] = analogRead(A2);
     
     values_1[i] = map_double(adc_out_1[i], 0, 2059, -311.15, 311.15);
     values_2[i] = map_double(adc_out_2[i], 0, 2059, -311.15, 311.15);
     values_3[i] = map_double(adc_out_3[i], 0, 2059, -311.15, 311.15);

     delayMicroseconds(388); //400 uS Delay for 500 samples for 1 cycle
    }
    //Calculate 500-Point DFT
    Xr_1=0;    Xr_2=0;    Xr_3=0;
    Xi_1=0;    Xi_2=0;    Xi_3=0;
    Phasor_Magnitude_1=0;    Phasor_Magnitude_2=0;    Phasor_Magnitude_3=0;
    Phasor_Angle_1=0;    Phasor_Angle_2=0;    Phasor_Angle_3=0;
    for (int n=0;n<N;n++)
    {
      Xr_1=Xr_1+values_1[n]*cos((2*pi*n)/N);
      Xi_1=Xi_1+values_1[n]*sin((2*pi*n)/N);
      
      Xr_2=Xr_2+values_2[n]*cos((2*pi*n)/N);
      Xi_2=Xi_2+values_2[n]*sin((2*pi*n)/N);
      
      Xr_3=Xr_3+values_3[n]*cos((2*pi*n)/N);
      Xi_3=Xi_3+values_3[n]*sin((2*pi*n)/N);
    }
    
    Xr_1=(sqrt(2)/N)*Xr_1;
    Xr_2=(sqrt(2)/N)*Xr_2;
    Xr_3=(sqrt(2)/N)*Xr_3;
    
    Xi_1=-(sqrt(2)/N)*Xi_1;
    Xi_2=-(sqrt(2)/N)*Xi_2;
    Xi_3=-(sqrt(2)/N)*Xi_3;
    
    Phasor_Magnitude_1=sqrt(Xr_1*Xr_1+Xi_1*Xi_1);
    Phasor_Magnitude_2=sqrt(Xr_2*Xr_2+Xi_2*Xi_2);
    Phasor_Magnitude_3=sqrt(Xr_3*Xr_3+Xi_3*Xi_3);
    
    Phasor_Angle_1=atan2(Xi_1,Xr_1);//double atan2(double y, double x)
    Phasor_Angle_2=atan2(Xi_2,Xr_2);//The atan2() function returns the arc tangent of y/x, in the range [-pi, +pi] radians.
    Phasor_Angle_3=atan2(Xi_3,Xr_3);
    
    //Calculate Phasor Angle in Degree
    Phasor_Angle_1=(Phasor_Angle_1*4068)/71;
    Phasor_Angle_2=(Phasor_Angle_2*4068)/71;
    Phasor_Angle_3=(Phasor_Angle_3*4068)/71;
    
    Serial.print("Phasor Magnitude for Phase 1 is: ");
    Serial.println(float(Phasor_Magnitude_1));
    Serial.print("Phasor Magnitude for Phase 2 is: ");
    Serial.println(float(Phasor_Magnitude_2));
    Serial.print("Phasor Magnitude for Phase 3 is: ");
    Serial.println(float(Phasor_Magnitude_3));
    
    Serial.print("Phase 1 Angle is: ");
    Serial.println(float(Phasor_Angle_1));
    Serial.print("Phase 2 Angle is: ");
    Serial.println(float(Phasor_Angle_2));
    Serial.print("Phase 3 Angle is: ");
    Serial.println(float(Phasor_Angle_3));
}
float map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
