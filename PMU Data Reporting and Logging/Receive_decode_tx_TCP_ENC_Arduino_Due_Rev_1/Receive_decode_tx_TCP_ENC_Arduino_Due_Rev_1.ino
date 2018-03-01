#include <SPI.h>
#include <UIPEthernet.h>

EthernetServer server = EthernetServer(80);

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
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,44,244);
  Ethernet.begin(mac,myIP);
  server.begin();
  
  Serial2.begin(921600);
  Serial3.begin(921600);
  //SerialUSB.begin(115200);
  Serial.begin(460800);
  Serial.println("Ready");
  print_headers();
//  // Enable pull-ups to avoid floating inputs
//    digitalWrite(A0, HIGH);
//    digitalWrite(A1, HIGH);
//    digitalWrite(A2, HIGH);
  
}
int count=0;
void loop() {
  display_on_tcp();
  // put your main code here, to run repeatedly:
  if (Serial3.available()) {
    int c = Serial3.read();
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
      
      
      //First Display Serially
      Display_Phasors_on_Serial_Terminal();
      transmit_phasors_LCD();
      //Display_Phasors_on_SerialUSB_Terminal();
      //Transmit Analog Voltages
      //analogread_pins_SerialUSB();
      //analogread_pins_Serial();
      //display_on_tcp();

      
      
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

//void Display_Phasors_on_Serial_Terminal(){
//  Serial.print("Phasor Magnitude for Phase 1 is: ");
//  Serial.println(float(Phasor_Magnitude_1/100));
//  Serial.print("Phasor Magnitude for Phase 2 is: ");
//  Serial.println(float(Phasor_Magnitude_2/100));
//  Serial.print("Phasor Magnitude for Phase 3 is: ");
//  Serial.println(float(Phasor_Magnitude_3/100));
//  
//  Serial.print("Phase 1 Angle is: ");
//  Serial.println(Phasor_Angle_Degree_1);
//  Serial.print("Phase 2 Angle is: ");
//  Serial.println(Phasor_Angle_Degree_2);
//  Serial.print("Phase 3 Angle is: ");
//  Serial.println(Phasor_Angle_Degree_3);
//
//  Serial.print("Latitude ");
//  Serial.print(Latitude/1000);
//  Serial.print(" ");
//  Serial.print("Longitude ");
//  Serial.print(Longitude/1000);
//  
//  Serial.print(" ");
//  
//  if (Hour < 10) Serial.print(F("0"));
//  Serial.print(Hour);
//  Serial.print(F(":"));
//  if (Minute < 10) Serial.print(F("0"));
//  Serial.print(Minute);
//  Serial.print(F(":"));
//  if (Second < 10) Serial.print(F("0"));
//  Serial.print(Second);
//  Serial.print(F(":"));
//  Serial.print(MilliSecond);
//  
//  Serial.print(" ");
//
//  if (Day < 10) Serial.print(F("0"));
//  Serial.print(Day);
//  Serial.print(F("/"));
//  if (Month < 10) Serial.print(F("0"));
//  Serial.print(Month);
//  Serial.print(F("/"));
//  Serial.println(Year);
//  Serial.println("");
//}

void transmit_phasors_LCD() {
  Serial2.write('!');
  Serial2.print(int(Phasor_Magnitude_1));
  Serial2.write('"');
  Serial2.print(int(Phasor_Magnitude_2));
  Serial2.write('#');
  Serial2.print(int(Phasor_Magnitude_3));
  Serial2.write('$');
  
  //Calculate angle i.e. 2pi's complement to be sent
  if(Phasor_Angle_1<0)
  Phasor_Angle_1=Phasor_Angle_1+6.286;
  if(Phasor_Angle_2<0)
  Phasor_Angle_2=Phasor_Angle_2+6.286;
  if(Phasor_Angle_3<0)
  Phasor_Angle_3=Phasor_Angle_3+6.286;
  Serial2.print(int(Phasor_Angle_1*100));
  Serial2.write('%');
  Serial2.print(int(Phasor_Angle_2*100));
  Serial2.write('&');
  Serial2.print(int(Phasor_Angle_3*100));
  Serial2.write('(');
  
  Serial2.print(Day);
  Serial2.write(')');
  Serial2.print(Month);
  Serial2.write('*');
  Serial2.print(Year);
  
  Serial2.write('+');
  Serial2.print(Hour);
  Serial2.write(',');
  Serial2.print(Minute);
  Serial2.write('-');
  Serial2.print(Second);
  Serial2.write('.');
  Serial2.print(MilliSecond);
  Serial2.write('/');
  
  Serial2.print(int(Latitude));
  Serial2.write(':');
  Serial2.print(int(Longitude));
  Serial2.write(';');
  
}

void display_on_tcp(){
  size_t size;

  if (EthernetClient client = server.available())
    {
      while((size = client.available()) > 0)
        {
          uint8_t* msg = (uint8_t*)malloc(size);
          size = client.read(msg,size);
          Serial.write(msg,size);
          free(msg);
//        Serial.println();
//        Serial.println("TCP Request Received");
//        Serial.println();
        }
      client.println("DATA from PMU 1 in NITR Simulation and Computing Lab!");
      //
      client.print("Phasor Magnitude for Phase 1 is: ");
      client.println(float(Phasor_Magnitude_1/100));
      client.print("Phasor Magnitude for Phase 2 is: ");
      client.println(float(Phasor_Magnitude_2/100));
      client.print("Phasor Magnitude for Phase 3 is: ");
      client.println(float(Phasor_Magnitude_3/100));
      
      client.print("Phase 1 Angle is: ");
      client.println(Phasor_Angle_Degree_1);
      client.print("Phase 2 Angle is: ");
      client.println(Phasor_Angle_Degree_2);
      client.print("Phase 3 Angle is: ");
      client.println(Phasor_Angle_Degree_3);
      
      client.print("Latitude ");
      client.print(Latitude/1000);
      client.print(" ");
      client.print("Longitude ");
      client.print(Longitude/1000);
      
      client.print(" ");
      
      if (Hour < 10) client.print(F("0"));
      client.print(Hour);
      client.print(F(":"));
      if (Minute < 10) client.print(F("0"));
      client.print(Minute);
      client.print(F(":"));
      if (Second < 10) client.print(F("0"));
      client.print(Second);
      client.print(F(":"));
      client.print(MilliSecond);
      
      client.print(" ");
    
      if (Day < 10) client.print(F("0"));
      client.print(Day);
      client.print(F("/"));
      if (Month < 10) client.print(F("0"));
      client.print(Month);
      client.print(F("/"));
      client.println(Year);
      client.println("");


      client.stop();
    }
}

