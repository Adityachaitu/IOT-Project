const int ldrPin = A0;
const int led = D1;
int threshold = 850;

void setup() {
  Serial.begin(115200);
  pinMode(ldrPin, INPUT);
  pinMode(led, OUTPUT);

}

void loop() {
  int rawData = analogRead(ldrPin);
  Serial.println(rawData);

  if (rawData >= threshold){
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }

  delay(100);
}
