#include <math.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20,20,4);  // set the LCD address to 0x20 for a 20 chars and 4 line display

boolean Calculate_A_Phasor = false;
//boolean timing = false;
int adc_out[500];
double values[500];
double Zi, Zr, Phasor_Magnitude, Phasor_Magnitude_RMS, Phasor_Angle, Phasor_Angle_Degree;

int N = 500;
void setup()
{
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("  Debashish's PMU  ");
  
  Serial.begin(115200);
  //pinMode(pinout, OUTPUT);
  //pinMode(2, INPUT);
  //attachInterrupt(24, CalcPhasor, RISING);//To calculate 10 Phasors Per Second as per GPS Disciplined Oscillator
  attachInterrupt(22, CalcPhasor, RISING); //To calculate 1 Phasor Per Second as per PPS from GPS
  //analogReadResolution(12);
  
}

void loop()
{

  if (Calculate_A_Phasor == true)
  {
    //Algorithm to calculate Phasor
    Zi = 0;
    Zr = 0;
    //Take 20 samples for 1 cycle Window To calculate phasor
    for (int i = 0; i < N; i++)
    {
      //Read adc
      adc_out[i] = analogRead(A0);
      delayMicroseconds(40); //40 uS Delay for 500 samples for 1 cycle
    }
    //Debug
    Serial.print("The Raw ADC Value is ");
    for (int i = 0; i < N; i++)
    {
      Serial.print(adc_out[i]);
      Serial.print(",");
    }

    //Map Raw ADC Value (Integer) into Voltage (-318 to +318)
    //value=adc_out;//Replace this line with proper Mapping
    for (int i = 0; i < N; i++)
    {
      values[i] = map_double(adc_out[i], 82, 913, -306.8, 306.8);
      //Serial.print(",");
    }
    //Debug
    Serial.print("The Mapped Voltage Samples are ");
    for (int i = 0; i < N; i++)
    {
      Serial.print(values[i]);
      Serial.print(",");
    }

    for (int i = 0; i < N; i++)
    {
      Zi = Zi + (values[i] * (sin(.0174*(360 * ((i + 1) / N)))));
      Zr = Zr + (values[i] * (cos(.0174*(360 * ((i + 1) / N)))));
    }
    //Debug Zi and Zr
    Serial.print("The Value of Zi is: ");
    Serial.println(Zi);
    Serial.print("The Value of Zr is: ");
    Serial.println(Zr);

    Phasor_Magnitude = sqrt((Zi * Zi) + (Zr * Zr));
    Phasor_Magnitude_RMS = Phasor_Magnitude*(2/N)*(1.414);
    Phasor_Angle = atan2(Zi, Zr);//Angle in Radians
    Phasor_Angle_Degree = 57.272*Phasor_Angle;//Phasor angle in Degrees
    //
    //
    Serial.print("The RMS Magnitude of Phasor is ");
    Serial.println(Phasor_Magnitude_RMS);
    Serial.print("The Phasor Angle is ");
    Serial.println(Phasor_Angle_Degree);
    
    //Print on LCD
    lcd.setCursor(0,1);
    lcd.print(" Magnitude: ");
    lcd.setCursor(13,1);//Clear Previous Value
    lcd.print("       ");
    lcd.setCursor(13,1);
    lcd.print(Phasor_Magnitude_RMS);
    
    lcd.setCursor(0,2);
    lcd.print(" Angle: ");
    lcd.setCursor(13,2);
    lcd.print("       ");
    lcd.setCursor(13,2);
    lcd.print(Phasor_Angle_Degree);

    //Reset the Boolean Flag
    Calculate_A_Phasor = false;
  }
}

void CalcPhasor() //interrupt routine //Set a Flag for a Phasor Calculation
{
  Calculate_A_Phasor = true;
}

//A Double mapping function
float map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


