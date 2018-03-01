//float Voltage100[100];
float SamplesR[2500];
float SamplesY[2500];
float SamplesB[2500];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  //delay(3000);
   for (int i = 0; i < 2500; i++)
        {
          //float sample=analogRead(A1);
          SamplesR[i] = analogRead(A0);
          SamplesY[i] = analogRead(A1);
          SamplesB[i] = analogRead(A2);
          //float sample_mapped=map(sample, 230, 836, -325, 325);
          //Voltage100[i] = sample_mapped; 
          //delayMicroseconds(400);
          //delayMicroseconds();
        }
       

 Serial.println("2500 Raw ADC VOLTAGE SAMPLES R Phase");
 for(int i=0;i<2500;i++)
 {
   Serial.print(SamplesR[i]);
   Serial.print(",");
 }
 Serial.println();
 Serial.println("2500 Raw ADC VOLTAGE SAMPLES Y Phase");
 for(int i=0;i<2500;i++)
 {
   Serial.print(SamplesY[i]);
   Serial.print(",");
 }
 Serial.println();
 Serial.println("2500 Raw ADC VOLTAGE SAMPLES B Phase");
 for(int i=0;i<2500;i++)
 {
   Serial.print(SamplesB[i]);
   Serial.print(",");
 }
 Serial.println();
 delay(60000);
}
