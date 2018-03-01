#include <UTFT.h>
extern uint8_t BigFont[];// Declare which fonts we will be using
UTFT myGLCD(CTE70,25,26,27,28);

bool display_now_on_lcd=false;

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
  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  init_LCD();
  Serial3.begin(921600);
  attachInterrupt(8, display_on_lcd, RISING);
    
}
int count=0;
void loop() {
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
      Phasor_Angle_1=Phasor_Angle_1/100;
      Phasor_Angle_2=Phasor_Angle_2/100;
      Phasor_Angle_3=Phasor_Angle_3/100;
      
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
      
      //Interrupt Driven LCD Display
      if(display_now_on_lcd==true){
        Display_on_LCD();
        display_now_on_lcd=false;
      }
        
      
      
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

void init_LCD(){
  //LCD Size 800:480
  //0,0   799,0
  //0,479 799,479 
  myGLCD.clrScr();
  myGLCD.setColor(0, 255, 0);
  myGLCD.print("* Phasor Measurement Unit Local Display *", CENTER, 1);
  myGLCD.setColor(255, 153, 51);
  myGLCD.print("!!! Developed by Debashish Mohapatra !!!", CENTER, 462);

  
  
  //Display Phasors Data
  //Print Static Lines
  myGLCD.setColor(255, 255, 255);//White
  myGLCD.print(" Phasor 1 Magnitude: ", LEFT,24);
  myGLCD.print(" Phasor 2 Magnitude: ", LEFT,41);
  myGLCD.print(" Phasor 3 Magnitude: ", LEFT,58);
  myGLCD.print(" Phasor 1 Angle: ", LEFT,75);
  myGLCD.print(" Phasor 2 Angle: ", LEFT,92);
  myGLCD.print(" Phasor 3 Angle: ", LEFT,109);
  
}
  
void Display_on_LCD(){
  myGLCD.setColor(153, 255, 255);//White-Blue
  //Print Variables
  myGLCD.printNumF(Phasor_Magnitude_1/100, LEFT, 330,24);
  myGLCD.printNumF(Phasor_Magnitude_2/100, LEFT, 330,41);
  myGLCD.printNumF(Phasor_Magnitude_3/100, LEFT, 330,58);
  
  myGLCD.setColor(153, 255, 204);
  myGLCD.printNumF(Phasor_Angle_Degree_1, LEFT, 330,75);
  myGLCD.printNumF(Phasor_Angle_Degree_2, LEFT, 330,92);
  myGLCD.printNumF(Phasor_Angle_Degree_3, LEFT, 330,109);
  
  //Print GPS UTC Time Info
  myGLCD.setColor(255, 255, 51);
  String Date=String(" Date: ")+String(Day)+"/"+String(Month)+"/"+String(Year);
  //myGLCD.print("                    ", LEFT,130);//Clear The line
  myGLCD.print(Date, LEFT,130);
  
  myGLCD.setColor(255, 51, 255);
  String Time=String(" UTC Time: ")+String(Hour)+":"+String(Minute)+":"+String(Second)+":"+String(MilliSecond);
  myGLCD.print("                              ", LEFT,148);//Clear The line
  myGLCD.print(Time, LEFT,148);
  myGLCD.setColor(204, 153, 255);
  String Location=String(" Location ")+String("Latitude: ")+String(Latitude/1000)+String(" Longitude: ")+String(Longitude/1000);
  myGLCD.print(Location, LEFT,168);
  
  
}
void display_on_lcd(){//ISR

  display_now_on_lcd = true;
}
  
    
