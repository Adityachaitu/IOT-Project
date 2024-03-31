#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN D5 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN D6    // Arduino pin tied to echo pin on the ultrasonic sensor.
#define SERVO_PIN D1   // Arduino pin tied to the servo motor.
#define CHANNEL_ID 123456 // Your ThingSpeak channel ID
#define API_KEY "SMXENAOC15LVLOCC" // Your ThingSpeak API Key
#define WIFI_SSID "Amrita_CHN" // Your WiFi SSID
#define WIFI_PASSWORD "amrita@321" // Your WiFi password

Servo servo;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, 200); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN); // Attaching the servo motor to the pin.
  connectToWiFi();
}

void loop() {
  int distance = sonar.ping_cm(); // Getting the distance from the ultrasonic sensor in centimeters.
  Serial.println(distance);

  // Check if the distance is less than the threshold value (in centimeters).
  if (distance < 20) {
    // Activate servo motor.
    servo.write(90); // Assuming 90 is the angle for the motor to be active.
    delay(500); // Adjust this delay as needed for your application.
    
    // Log data to ThingSpeak.
    logToThingSpeak(distance);
  } else {
    servo.write(0); // Deactivate servo motor.
  }
  delay(1000); // Adjust delay as needed for your application.
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to WiFi network.

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}

void logToThingSpeak(int distance) {
  WiFiClient client;
  if (client.connect("api.thingspeak.com", 80)) {
    String url = "/update?api_key=";
    url += API_KEY;
    url += "&field1=";
    url += String(distance); // Log distance to ThingSpeak field.
    client.print("GET " + url + " HTTP/1.1\r\n");
    client.print("Host: api.thingspeak.com\r\n");
    client.print("Connection: close\r\n\r\n");
    delay(1000); // Wait for server response
    client.stop();
  } else {
    Serial.println("Connection to ThingSpeak failed!");
  }
}
