#define IDLE  0
#define RECEIVING1 1
#define RECEIVING2 2
#define RECEIVING3 3
#define RECEIVING4 4
#define RECEIVING5 5
#define RECEIVING6 6
#define RECEIVING7 7
#define RECEIVING8 8
#define RECEIVING9 9
#define RECEIVING10 10
#define RECEIVING11 11
#define RECEIVING12 12
#define RECEIVING13 13
#define RECEIVING14 14
#define RECEIVING15 15

int Year;
int Month;
int Day;
int Hour;
int Minute;
int Second;
int MilliSecond;

float Phasor_Magnitude_1;
float Phasor_Magnitude_2;
float Phasor_Magnitude_3;

float Phasor_Angle_1,Phasor_Angle_Degree_1;
float Phasor_Angle_2,Phasor_Angle_Degree_2;
float Phasor_Angle_3,Phasor_Angle_Degree_3;


float Latitude;
float Longitude;

byte status = IDLE;

void setup() {
  Serial2.begin(921600);
  Serial3.begin(921600);
  Serial.begin(460800);
  Serial.println("Ready");
  print_headers();
}
int count=0;
void loop() {
  
  if (Serial2.available()) {
    int c = Serial2.read();
    if (status == RECEIVING1 && c >= '0' && c <= '9') {
      Phasor_Magnitude_1 = Phasor_Magnitude_1 * 10 + (c - '0');
    } else if (status == RECEIVING2 && c >= '0' && c <= '9') {
      Phasor_Magnitude_2 = Phasor_Magnitude_2 * 10 + (c - '0');
    } else if (status == RECEIVING3 && c >= '0' && c <= '9') {
      Phasor_Magnitude_3 = Phasor_Magnitude_3 * 10 + (c - '0');
    } else if (status == RECEIVING4 && c >= '0' && c <= '9') {
      Phasor_Angle_1 = Phasor_Angle_1 * 10 + (c - '0');
    } else if (status == RECEIVING5 && c >= '0' && c <= '9') {
      Phasor_Angle_2 = Phasor_Angle_2 * 10 + (c - '0');
    } else if (status == RECEIVING6 && c >= '0' && c <= '9') {
      Phasor_Angle_3 = Phasor_Angle_3 * 10 + (c - '0');
    } else if (status == RECEIVING7 && c >= '0' && c <= '9') {
      Day = Day * 10 + (c - '0');
    } else if (status == RECEIVING8 && c >= '0' && c <= '9') {
      Month = Month * 10 + (c - '0');
    } else if (status == RECEIVING9 && c >= '0' && c <= '9') {
      Year = Year * 10 + (c - '0');
    } else if (status == RECEIVING10 && c >= '0' && c <= '9') {
      Hour = Hour * 10 + (c - '0');
    } else if (status == RECEIVING11 && c >= '0' && c <= '9') {
      Minute = Minute  * 10 + (c - '0');
    } else if (status == RECEIVING12 && c >= '0' && c <= '9') {
      Second = Second * 10 + (c - '0');
    }  else if (status == RECEIVING13 && c >= '0' && c <= '9') {
      MilliSecond = MilliSecond * 10 + (c - '0');
    }  else if (status == RECEIVING14 && c >= '0' && c <= '9') {
      Latitude = Latitude * 10 + (c - '0');
    }  else if (status == RECEIVING15 && c >= '0' && c <= '9') {
      Longitude = Longitude * 10 + (c - '0');
    }



    else if (status == RECEIVING1 && c == '"') {
      status = RECEIVING2;
    } else if (status == RECEIVING2 && c == '#') {
      status = RECEIVING3;
    } else if (status == RECEIVING3 && c == '$') {
      status = RECEIVING4;
    } else if (status == RECEIVING4 && c == '%') {
      status = RECEIVING5;
    } else if (status == RECEIVING5 && c == '&') {
      status = RECEIVING6;
    } else if (status == RECEIVING6 && c == '(') {
      status = RECEIVING7;
    } else if (status == RECEIVING7 && c == ')') {
      status = RECEIVING8;
    } else if (status == RECEIVING8 && c == '*') {
      status = RECEIVING9;
    } else if (status == RECEIVING9 && c == '+') {
      status = RECEIVING10;
    } else if (status == RECEIVING10 && c == ',') {
      status = RECEIVING11;
    } else if (status == RECEIVING11 && c == '-') {
      status = RECEIVING12;
    } else if (status == RECEIVING12 && c == '.') {
      status = RECEIVING13;
    } else if (status == RECEIVING13 && c == '/') {
      status = RECEIVING14;
    } else if (status == RECEIVING14 && c == ':') {
      status = RECEIVING15;
    } else if (c == ';') {
      status = IDLE;
      
      //Remote value Received completely, Now display it
      
      //Calculate Phasor Angles into Float
      Phasor_Angle_1=Phasor_Angle_1/1000;
      Phasor_Angle_2=Phasor_Angle_2/1000;
      Phasor_Angle_3=Phasor_Angle_3/1000;
      
      //Calculate Phasor Angle using reverse 2pi's complement
      if(Phasor_Angle_1>3.143)
      Phasor_Angle_1=Phasor_Angle_1-6.286;
      if(Phasor_Angle_2>3.143)
      Phasor_Angle_2=Phasor_Angle_2-6.286;
      if(Phasor_Angle_3>3.143)
      Phasor_Angle_3=Phasor_Angle_3-6.286;
      
      //Calculate Angles in Degrees
      Phasor_Angle_Degree_1 = (Phasor_Angle_1 * 4068) / 71;
      Phasor_Angle_Degree_2 = (Phasor_Angle_2 * 4068) / 71;
      Phasor_Angle_Degree_3 = (Phasor_Angle_3 * 4068) / 71;

      Display_Phasors_on_Serial_Terminal();
      transmit_phasors_LCD();

      
      
    }else if (c=='!'){
      status = RECEIVING1;
      
      //Reset the variables to Zero
      Year=0;
      Month=0;
      Day=0;
      
      Hour=0;
      Minute=0;
      Second=0;
      MilliSecond=0;
      
      Phasor_Angle_1=0;
      Phasor_Angle_2=0;
      Phasor_Angle_3=0;
      
      Phasor_Magnitude_1=0;
      Phasor_Magnitude_2=0;
      Phasor_Magnitude_3=0;
      
      Phasor_Angle_Degree_1=0;
      Phasor_Angle_Degree_2=0;
      Phasor_Angle_Degree_3=0;
      
      Latitude=0;
      Longitude=0;
    }
  }
}
void print_headers(){
  Serial.print("Rm");
  Serial.print("\t");
  Serial.print("\t");  
  Serial.print("Ra");
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("Ym");
  Serial.print("\t"); 
  Serial.print("\t"); 
  Serial.print("Ya");
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("Bm");
  Serial.print("\t");  
  Serial.print("\t");
  Serial.print("Ba");
  Serial.print("\t");
  Serial.print("\t");
  Serial.println("Time in mS");
  Serial.println("__________________________________________________________");
}
void Display_Phasors_on_Serial_Terminal(){
  Serial.print(float(Phasor_Magnitude_1/100));
  Serial.print("\t");
  Serial.print(Phasor_Angle_Degree_1);
  Serial.print("\t");  
  Serial.print(float(Phasor_Magnitude_2/100));
  Serial.print("\t");  
  Serial.print(Phasor_Angle_Degree_2);
  Serial.print("\t");  
  Serial.print(float(Phasor_Magnitude_3/100));
  Serial.print("\t");  
  Serial.print(Phasor_Angle_Degree_3);
  Serial.print("\t");  
  //Serial.print(MilliSecond);
  Serial.print(millis());
  Serial.println();
}


void transmit_phasors_LCD() {
  Serial3.write('!');
  Serial3.print(int(Phasor_Magnitude_1));
  Serial3.write('"');
  Serial3.print(int(Phasor_Magnitude_2));
  Serial3.write('#');
  Serial3.print(int(Phasor_Magnitude_3));
  Serial3.write('$');
  
  //Calculate angle i.e. 2pi's complement to be sent
  if(Phasor_Angle_1<0)
  Phasor_Angle_1=Phasor_Angle_1+6.286;
  if(Phasor_Angle_2<0)
  Phasor_Angle_2=Phasor_Angle_2+6.286;
  if(Phasor_Angle_3<0)
  Phasor_Angle_3=Phasor_Angle_3+6.286;
  Serial3.print(int(Phasor_Angle_1*100));
  Serial3.write('%');
  Serial3.print(int(Phasor_Angle_2*100));
  Serial3.write('&');
  Serial3.print(int(Phasor_Angle_3*100));
  Serial3.write('(');
  
  Serial3.print(Day);
  Serial3.write(')');
  Serial3.print(Month);
  Serial3.write('*');
  Serial3.print(Year);
  
  Serial3.write('+');
  Serial3.print(Hour);
  Serial3.write(',');
  Serial3.print(Minute);
  Serial3.write('-');
  Serial3.print(Second);
  Serial3.write('.');
  Serial3.print(MilliSecond);
  Serial3.write('/');
  
  Serial3.print(int(Latitude));
  Serial3.write(':');
  Serial3.print(int(Longitude));
  Serial3.write(';');
  
}

