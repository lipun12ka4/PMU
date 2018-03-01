void setup() {
  // put your setup code here, to run once:
Serial.begin(460800);
//while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }
// Enable pull-ups to avoid floating inputs
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
   
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(A0));
  Serial.print(',');
  Serial.print(analogRead(A1));
  Serial.print(',');
  Serial.print(analogRead(A2));
  Serial.print('\r');

}
