#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,20,4);  // set the LCD address to 0x20 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");
}

void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Debashish Line 1");
  lcd.setCursor(1,1);
  lcd.print("Line 2 Mohapatra");
  lcd.setCursor(2,2);
  lcd.print("Debashish Line 3");
  lcd.setCursor(3,3);
  lcd.print("Line 4 Mohapatra");
}
