const int ledPin =  13;
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 500;
void setup() {
  Serial.begin(115200);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  pinMode(ledPin, OUTPUT);

}

void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(ledPin, ledState);
  }
  Serial.print(analogRead(A0));
  Serial.print(',');
  Serial.print(analogRead(A1));
  Serial.print(',');
  Serial.print(analogRead(A2));
  Serial.print('\r');
  delayMicroseconds(33);

}
