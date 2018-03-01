#include <Time.h>             // Time Library
#include <TinyGPS++.h>        // GPS Library

static const uint32_t GPSBaud = 38400;
boolean Calculate_A_Phasor = false;
boolean get_time_on_pps = false;
// The TinyGPS++ object
TinyGPSPlus gps;

// Serial connection to the GPS device
#define Serial_GPS Serial3
#define SerialTx Serial2

time_t prevDisplay = 0; // Count for when time last displayed
int Year;
byte Month;
byte Day;
byte Hour;
byte Minute;
byte Second;

//Phasor Estimation Variable Declaration
#define WindowSize 64 //i.e. 64 samples per second
int N = WindowSize;   //Sampling frequency 3200 Hz
long double pi = 3.143;

long double adc_out_1[WindowSize];
long double adc_out_2[WindowSize];
long double adc_out_3[WindowSize];

long double values_1[WindowSize];
long double values_2[WindowSize];
long double values_3[WindowSize];


long double Xi_1, Xr_1, Phasor_Magnitude_1, Phasor_Angle_1, Phasor_Angle_Degree_1;
long double Xi_2, Xr_2, Phasor_Magnitude_2, Phasor_Angle_2, Phasor_Angle_Degree_2;
long double Xi_3, Xr_3, Phasor_Magnitude_3, Phasor_Angle_3, Phasor_Angle_Degree_3;

unsigned long int calculation_start_millis;
unsigned long int calculation_finish_millis;

unsigned long int pps_time_millis;
unsigned long int phasor_stamp_millis;

void setup()
{ adc_setup();
  SerialTx.begin(921600);//for transmitting Phasors
  Serial_GPS.begin(GPSBaud); // Start GPS Serial Connection
  smartDelay(1000);
  delay(2000);
  analogReadResolution(12);
  attachInterrupt(22, aquire, RISING);
  attachInterrupt(23, attach_pps_time, RISING);
}
// Circular buffer, power of two.
#define BUFSIZE 0x40 //64 samples buffer
#define BUFMASK 0x3F
volatile int R [BUFSIZE] ;
volatile int Y [BUFSIZE] ;
volatile int B [BUFSIZE] ;
volatile int sptr = 0 ;
volatile int isr_count = 0 ;

void aquire() {
  ADC->ADC_CR |= 0b10; //start conversion
  while (!(ADC->ADC_ISR & 0b11100000)); //wait for conversion to end
  int Rval = ADC->ADC_CDR[7];
  int Yval = ADC->ADC_CDR[6];
  int Bval = ADC->ADC_CDR[5];

  R[sptr] = Rval;
  Y[sptr] = Yval;
  B[sptr] = Bval;
  sptr = (sptr + 1) & BUFMASK;
  isr_count ++ ;
}

void adc_setup()
{
  //ADC setup
  ADC->ADC_WPMR &= 0xFFFE; //disable write protect
  ADC->ADC_CHER = 0b11100000; //Enable AD7,AD6,AD5 or CH7,Ch6,Ch5 or PA16,PA24,PA23 or A0,A1 and A2     |
  ADC->ADC_MR &= 0b11111111000000000000011100000000;//Fast i.e. about 4mS for 2500 Conversions on three channels
  ADC->ADC_MR |= 0b00000000000100100000000000000000; //software trigger, hi res, no sleep, not free running
  ADC->ADC_IER = 0b11100000; //enable interrupt
  ADC->ADC_IMR = 0b11100000; //enable interrupt in mask
  ADC->ADC_CR |= 0b10; //start first conversion
}

void loop()
{
  if (get_time_on_pps == true)
  {
    pps_time_millis = millis();
    GPS_Timezone_Adjust();  // Call Time Adjust Function
    get_time_on_pps = false;
  }
  if (isr_count == 64)
  {
    Calculate_A_Phasor = true;
    isr_count = 0;
  }
  if (Calculate_A_Phasor == true)
  {
    calc_phasor();
    transmit_phasors_on_SerialTx();
    Calculate_A_Phasor = false;
  }
  smartDelay(0);
}

void attach_pps_time()
{
  get_time_on_pps = true;
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

  // -- Delete this section if not displaying time ------- //
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) {
      prevDisplay = now();

    }
  }
  // -- Also delete the SerialClockDisplay()function ---- //
  smartDelay(0);

} // Keep


static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial_GPS.available())
      gps.encode(Serial_GPS.read());
  } while (millis() - start < ms);
}

//Phasor calculation function
void calc_phasor() {
  //copy buffer to SampleWindow for calculation
  for (int i = 0; i < 64; i++)
  {
    adc_out_1[i] = R[i];
    adc_out_2[i] = Y[i];
    adc_out_3[i] = B[i];
  }
  calculation_start_millis = millis();
  for (int i = 0; i < N; i++)
  {
    values_1[i] = map_double(adc_out_1[i], 0, 2059, -311.15, 311.15);
    values_2[i] = map_double(adc_out_2[i], 0, 2059, -311.15, 311.15);
    values_3[i] = map_double(adc_out_3[i], 0, 2059, -311.15, 311.15);
    smartDelay(0);
  }

  //Calculate 64-Point DFT
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
  //
  calculation_finish_millis = millis();
  phasor_stamp_millis = calculation_start_millis - pps_time_millis;
  smartDelay(0);
}

float map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  smartDelay(0);
}

void transmit_phasors_on_SerialTx() {
  SerialTx.write('!');
  SerialTx.print(int(Phasor_Magnitude_1 * 100));
  SerialTx.write('"');
  SerialTx.print(int(Phasor_Magnitude_2 * 100));
  SerialTx.write('#');
  SerialTx.print(int(Phasor_Magnitude_3 * 100));
  SerialTx.write('$');

  //Calculate angle i.e. 2pi's complement to be sent
  if (Phasor_Angle_1 < 0)
    Phasor_Angle_1 = Phasor_Angle_1 + 6.286;
  if (Phasor_Angle_2 < 0)
    Phasor_Angle_2 = Phasor_Angle_2 + 6.286;
  if (Phasor_Angle_3 < 0)
    Phasor_Angle_3 = Phasor_Angle_3 + 6.286;
  SerialTx.print(int(Phasor_Angle_1 * 100));
  SerialTx.write('%');
  SerialTx.print(int(Phasor_Angle_2 * 100));
  SerialTx.write('&');
  SerialTx.print(int(Phasor_Angle_3 * 100));
  SerialTx.write('(');

  SerialTx.print(day());
  SerialTx.write(')');
  SerialTx.print(month());
  SerialTx.write('*');
  SerialTx.print(year());

  SerialTx.write('+');
  SerialTx.print(hour());
  SerialTx.write(',');
  SerialTx.print(minute());
  SerialTx.write('-');
  SerialTx.print(second());
  SerialTx.write('.');
  SerialTx.print(phasor_stamp_millis);
  SerialTx.write('/');

  //printFloatSerialTx(gps.location.lat(), gps.location.isValid(), 11, 4);
  float Latitude = (gps.location.lat());
  float Longitude = (gps.location.lng());
  //SerialTx.print((gps.location.lat(), 2)*100);
  SerialTx.print(int(Latitude * 1000));
  SerialTx.write(':');
  //SerialTx.print((gps.location.lng(), 2)*100);
  //printFloatSerialTx((gps.location.lng(), 4)*10000);
  SerialTx.print(int(Longitude * 1000));
  SerialTx.write(';');
}
