#include <math.h>
boolean Calculate_A_Phasor = false;
//boolean timing = false;
int adc_out;
double value,Zi,Zr,Phasor_Magnitude,Phasor_Angle;
int N=50;
void setup()
{
  Serial.begin(115200);
  //pinMode(pinout, OUTPUT);
  //pinMode(2, INPUT);
  //attachInterrupt(22, CalcPhasor, RISING);//To calculate 10 Phasors Per Second as per GPS Disciplined Oscillator
  attachInterrupt(24, CalcPhasor, RISING); //To calculate 1 Phasor Per Second as per PPS from GPS
}

void loop()
{

  if(Calculate_A_Phasor == true) 
  {
    //Algorithm to calculate Phasor
    Zi=0;
    Zr=0;
    //Take 20 samples for 1 cycle Window To calculate phasor
    for(int i=1;i<(N+1);i++)
    {
      //Read adc
      adc_out=analogRead(A0);
      
      //Map Raw ADC Value (Integer) into Voltage (-318 to +318)
      //value=adc_out;//Replace this line with proper Mapping
      float value=map(adc_out,82,913,-153.42,153.42);//This Is correct
      
      //Debug
      Serial.print("The Raw ADC Value is ");
      Serial.println(adc_out);
      Serial.print("Mapped ADC Value is ");
      Serial.println(value);
      
      Zi=Zi+(value*(sin(360*(i/N))));
      Zr=Zr+(value*(cos(360*(i/N))));
      delay(20/N);
    }
    Zi=(2/N)*Zi;
    Zr=(2/N)*Zr;
  
  Serial.print("The Value of Zi is: ");
  Serial.println(Zi);
  Serial.print("The Value of Zr is: ");
  Serial.println(Zr);
  
  Phasor_Magnitude=sqrt((Zi*Zi)+(Zr*Zr));
  Phasor_Angle=atan2(Zi,Zr);  
  
  
  Serial.print("The Magnitude of Phasor is ");
  Serial.println(Phasor_Magnitude);
  Serial.print("The Phasor Angle is ");
  Serial.println(Phasor_Angle);
   
   //Reset the Boolean Flag
  Calculate_A_Phasor = false;
  }
  

}

void CalcPhasor() //interrupt routine //Set a Flag for a Phasor Calculation
{
  Calculate_A_Phasor = true;
}

//A Double mapping function
//float map_double(double x, double in_min, double in_max, double out_min, double out_max)
//{
//  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
//}
