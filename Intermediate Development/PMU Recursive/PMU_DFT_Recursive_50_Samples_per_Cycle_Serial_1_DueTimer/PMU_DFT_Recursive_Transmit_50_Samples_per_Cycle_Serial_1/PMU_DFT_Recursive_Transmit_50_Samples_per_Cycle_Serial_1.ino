#include <Time.h>             // Time Library
#include <TinyGPS++.h>        // GPS Library
#include <DueTimer.h>
static const uint32_t GPSBaud = 38400;
//boolean Calculate_A_Phasor = false;
boolean take_samples = false;
boolean get_time_on_pps = false;
// The TinyGPS++ object
TinyGPSPlus gps;

// Serial connection to the GPS device
#define Serial_GPS Serial3

time_t prevDisplay = 0; // Count for when time last displayed
int Year;
byte Month;
byte Day;
byte Hour;
byte Minute;
byte Second;

//Phasoer Estimation Variable Declaration
int N = 50; //Sampling frequency 2500 Hz
long double pi = 3.143;

 long double adc_out_1[50];
 long double adc_out_2[50];
 long double adc_out_3[50];

long double adc_out_samples_1[50];
long double adc_out_samples_2[50];
long double adc_out_samples_3[50];

long double values_1[50];
long double values_2[50];
long double values_3[50];


long double Xi_1, Xr_1, Phasor_Magnitude_1, Phasor_Angle_1, Phasor_Angle_Degree_1;
long double Xi_2, Xr_2, Phasor_Magnitude_2, Phasor_Angle_2, Phasor_Angle_Degree_2;
long double Xi_3, Xr_3, Phasor_Magnitude_3, Phasor_Angle_3, Phasor_Angle_Degree_3;

unsigned long int calculation_start_millis;
unsigned long int calculation_finish_millis;
unsigned long int calculation_start_micros;
unsigned long int calculation_finish_micros;

unsigned long int acquisition_start_millis;
unsigned long int acquisition_finish_millis;
unsigned long int acquisition_start_micros;
unsigned long int acquisition_finish_micros;

unsigned long int transmission_start_millis;
unsigned long int transmission_finish_millis;
unsigned long int transmission_start_micros;
unsigned long int transmission_finish_micros;

unsigned long int pps_time_millis;
unsigned long int phasor_stamp_millis;

void setup()
{
  Serial.begin(460800);
  Serial1.begin(921600);
  //SerialUSB.begin(0);
  Serial_GPS.begin(GPSBaud); // Start GPS Serial Connection
  //attachInterrupt(22, set_time_on_pps, RISING);
  Serial.println("Waiting for GPS time ... ");
  smartDelay(1000);
  //delay(2000);
  analogReadResolution(12);
  //attachInterrupt(22, take_samples_trig, RISING);//sampling clock from GPSDO
  attachInterrupt(23, attach_pps_time, RISING); //PPS Time from GPS Module
  
  Timer6.attachInterrupt(take_samples_trig).start(400);
  Timer7.attachInterrupt(report_phasor).start(40000);
  
}
void report_phasor()
{
  calc_phasor();
  transmit_phasors_on_Serial1();
}
void loop()
{
  GPS_Timezone_Adjust();  // Call Time Adjust Function
  smartDelay(0);
}

void attach_pps_time()
{
  pps_time_millis = millis();
  //Reset Timers
  Timer1.stop();
  Timer2.stop();
  Timer1.start();
  Timer2.start();
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
}

//Phasor calculation function
void calc_phasor() {
  calculation_start_millis = millis();
  //Copy the samples arrays for calculation of phasor
  memcpy (adc_out_samples_1,adc_out_1,50);
  memcpy (adc_out_samples_2,adc_out_2,50);
  memcpy (adc_out_samples_3,adc_out_3,50);
  
  for (int i = 0; i < N; i++)
  {
    values_1[i] = map_double(adc_out_samples_1[i], 0, 2059, -311.15, 311.15);
    values_2[i] = map_double(adc_out_samples_2[i], 0, 2059, -311.15, 311.15);
    values_3[i] = map_double(adc_out_samples_3[i], 0, 2059, -311.15, 311.15);
    smartDelay(0);
  }

  //Calculate 50-Point DFT
  Xr_1 = 0;    Xr_2 = 0;    Xr_3 = 0;
  Xi_1 = 0;    Xi_2 = 0;    Xi_3 = 0;
  Phasor_Magnitude_1 = 0;    Phasor_Magnitude_2 = 0;    Phasor_Magnitude_3 = 0;
  Phasor_Angle_1 = 0;    Phasor_Angle_2 = 0;    Phasor_Angle_3 = 0;
  for (int n = 0; n < N; n++)
  {
    Xr_1 = Xr_1 + values_1[n] * cos((2 * pi * n) / N);
    Xi_1 = Xi_1 + values_1[n] * sin((2 * pi * n) / N);

    Xr_2 = Xr_2 + values_2[n] * cos((2 * pi * n) / N);
    Xi_2 = Xi_2 + values_2[n] * sin((2 * pi * n) / N);

    Xr_3 = Xr_3 + values_3[n] * cos((2 * pi * n) / N);
    Xi_3 = Xi_3 + values_3[n] * sin((2 * pi * n) / N);
    smartDelay(0);
  }

  Xr_1 = (sqrt(2) / N) * Xr_1;
  Xr_2 = (sqrt(2) / N) * Xr_2;
  Xr_3 = (sqrt(2) / N) * Xr_3;

  Xi_1 = -(sqrt(2) / N) * Xi_1;
  Xi_2 = -(sqrt(2) / N) * Xi_2;
  Xi_3 = -(sqrt(2) / N) * Xi_3;

  Phasor_Magnitude_1 = sqrt(Xr_1 * Xr_1 + Xi_1 * Xi_1);
  Phasor_Magnitude_2 = sqrt(Xr_2 * Xr_2 + Xi_2 * Xi_2);
  Phasor_Magnitude_3 = sqrt(Xr_3 * Xr_3 + Xi_3 * Xi_3);

  Phasor_Angle_1 = atan2(Xi_1, Xr_1); //double atan2(double y, double x)
  Phasor_Angle_2 = atan2(Xi_2, Xr_2); //The atan2() function returns the arc tangent of y/x, in the range [-pi, +pi] radians.
  Phasor_Angle_3 = atan2(Xi_3, Xr_3);

  //Calculate Phasor Angle in Degree
  Phasor_Angle_Degree_1 = (Phasor_Angle_1 * 4068) / 71;
  Phasor_Angle_Degree_2 = (Phasor_Angle_2 * 4068) / 71;
  Phasor_Angle_Degree_3 = (Phasor_Angle_3 * 4068) / 71;
  //calculate millisecond time stamp
  phasor_stamp_millis=calculation_start_millis - pps_time_millis;
  if(phasor_stamp_millis==0)
  {
    phasor_stamp_millis=1000;
  }
  
  smartDelay(0);
}

float map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  smartDelay(0);
}

void transmit_phasors_on_Serial1() {
  Serial1.write('!');
  Serial1.print(int(Phasor_Magnitude_1 * 100));
  Serial1.write('"');
  Serial1.print(int(Phasor_Magnitude_2 * 100));
  Serial1.write('#');
  Serial1.print(int(Phasor_Magnitude_3 * 100));
  Serial1.write('$');

  //Calculate angle i.e. 2pi's complement to be sent
  if (Phasor_Angle_1 < 0)
    Phasor_Angle_1 = Phasor_Angle_1 + 6.286;
  if (Phasor_Angle_2 < 0)
    Phasor_Angle_2 = Phasor_Angle_2 + 6.286;
  if (Phasor_Angle_3 < 0)
    Phasor_Angle_3 = Phasor_Angle_3 + 6.286;
  Serial1.print(int(Phasor_Angle_1 * 100));
  Serial1.write('%');
  Serial1.print(int(Phasor_Angle_2 * 100));
  Serial1.write('&');
  Serial1.print(int(Phasor_Angle_3 * 100));
  Serial1.write('(');

  Serial1.print(day());
  Serial1.write(')');
  Serial1.print(month());
  Serial1.write('*');
  Serial1.print(year());

  Serial1.write('+');
  Serial1.print(hour());
  Serial1.write(',');
  Serial1.print(minute());
  Serial1.write('-');
  Serial1.print(second());
  Serial1.write('.');
  Serial1.print(phasor_stamp_millis);
  Serial1.write('/');

  //printFloatSerial1(gps.location.lat(), gps.location.isValid(), 11, 4);
  float Latitude = (gps.location.lat());
  float Longitude = (gps.location.lng());
  //Serial1.print((gps.location.lat(), 2)*100);
  Serial1.print(int(Latitude * 1000));
  Serial1.write(':');
  //Serial1.print((gps.location.lng(), 2)*100);
  //printFloatSerial1((gps.location.lng(), 4)*10000);
  Serial1.print(int(Longitude * 1000));
  Serial1.write(';');

}

void Transmit_Phasors_on_Serial() {
  Serial.print("Phasor Magnitude for Phase 1 is: ");
  Serial.println(float(Phasor_Magnitude_1));
  Serial.print("Phasor Magnitude for Phase 2 is: ");
  Serial.println(float(Phasor_Magnitude_2));
  Serial.print("Phasor Magnitude for Phase 3 is: ");
  Serial.println(float(Phasor_Magnitude_3));

  Serial.print("Phase 1 Angle is: ");
  Serial.println(float(Phasor_Angle_Degree_1));
  Serial.print("Phase 2 Angle is: ");
  Serial.println(float(Phasor_Angle_Degree_2));
  Serial.print("Phase 3 Angle is: ");
  Serial.println(float(Phasor_Angle_Degree_3));

  Serial.print("Latitude ");
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  Serial.print("Longitude ");
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);

  if (hour() < 10) Serial.print(F("0"));
  Serial.print(hour());
  Serial.print(F(":"));
  if (minute() < 10) Serial.print(F("0"));
  Serial.print(minute());
  Serial.print(F(":"));
  if (second() < 10) Serial.print(F("0"));
  Serial.print(second());
  Serial.print(F(":"));
  Serial.print(phasor_stamp_millis);


  Serial.print(" ");

  if (day() < 10) Serial.print(F("0"));
  Serial.print(day());
  Serial.print(F("/"));
  if (month() < 10) Serial.print(F("0"));
  Serial.print(month());
  Serial.print(F("/"));
  Serial.println(year());

  smartDelay(0);

}

void GPS_Timezone_Adjust() {

  Year = gps.date.year();
  Month = gps.date.month();
  Day = gps.date.day();
  Hour = gps.time.hour();
  Minute = gps.time.minute();
  Second = gps.time.second();

  // Set Time from GPS data string
  setTime(Hour, Minute, Second, Day, Month, Year);
  // Calc current Time Zone time by offset value
  //adjustTime(5 * SECS_PER_HOUR + 1800);



  // -- Delete this section if not displaying time ------- //
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) {
      prevDisplay = now();

      //SerialClockDisplay();
      //lcd_clock_display();
    }
  }
  // -- Also delete the SerialClockDisplay()function ---- //
  smartDelay(0);

} // Keep



void SerialClockDisplay() {
  // Serial Monitor display of new calculated time -
  // once adjusted GPS time stored in now() Time Library
  // calculations or displays can be made.
  Serial.print("Latitude ");
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  Serial.print("Longitude ");
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);

  if (hour() < 10) Serial.print(F("0"));
  Serial.print(hour());
  Serial.print(F(":"));
  if (minute() < 10) Serial.print(F("0"));
  Serial.print(minute());
  Serial.print(F(":"));
  if (second() < 10) Serial.print(F("0"));
  Serial.print(second());
  Serial.print(F(":"));
  Serial.print(calculation_start_millis - pps_time_millis);


  Serial.print(" ");

  if (day() < 10) Serial.print(F("0"));
  Serial.print(day());
  Serial.print(F("/"));
  if (month() < 10) Serial.print(F("0"));
  Serial.print(month());
  Serial.print(F("/"));
  Serial.println(year());
  smartDelay(0);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial_GPS.available())
      gps.encode(Serial_GPS.read());
  } while (millis() - start < ms);
}

