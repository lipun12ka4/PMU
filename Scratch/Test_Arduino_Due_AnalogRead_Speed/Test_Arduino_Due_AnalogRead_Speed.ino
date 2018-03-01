//float Voltage100[100];
float SamplesR[20000];
unsigned long int time_before_read;
unsigned long int time_after_read;
unsigned long int time_taken;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

void loop() {
analogReadResolution(10);
delay(1000);
   time_before_read=millis();
   for (int i = 0; i < 20000; i++)
        {
          //float sample=analogRead(A1);
          SamplesR[i] = analogRead(A0);
        }
   time_after_read=millis();
   time_taken=time_after_read-time_before_read;
     
Serial.print("Time taken to read 20000 Samples with 10-bit resolution is: ");
Serial.println(time_taken);

delay(1000);

analogReadResolution(12);
delay(1000);
   time_before_read=millis();
   for (int i = 0; i < 20000; i++)
        {
          //float sample=analogRead(A1);
          SamplesR[i] = analogRead(A0);
        }
   time_after_read=millis();
   time_taken=time_after_read-time_before_read;
     
Serial.print("Time taken to read 20000 Samples with 12-bit Resolution is: ");
Serial.println(time_taken);
delay(5000);
}
