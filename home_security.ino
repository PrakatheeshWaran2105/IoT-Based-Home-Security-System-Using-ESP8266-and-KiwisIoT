/*
  Project: Smart Door and Motion Detection
  Board: NodeMCU ESP8266
  Platform: KiwisIoT
*/

#include <ESP8266WiFi.h>
#include <KiwisIoT.h>

// WiFi Configuration
const char* ssid = "YOUR_WIFI_NAME";
const char* pass = "YOUR_WIFI_PASSWORD";

// KiwisIoT Dashboard Topic ID
const char* topic = "YOUR_DASHBOARD_TOPIC_ID";

// KiwisIoT Initialization
KiwisIoT kiwisiot(ssid, pass, topic);

// Sensor Pins
#define PIR_PIN D7
#define REED_PIN D6
#define LED_PIN D0

int pirState;
int doorState;

unsigned long previousMillis = 0;
const unsigned long interval = 2000;

void setup() {

  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.println("Smart Door & Motion Detection");
  Serial.println("Connecting to KiwisIoT...");

  kiwisiot.begin();

  Serial.println("KiwisIoT Initialized!");
}

void loop() {

  // Maintain KiwisIoT connection
  kiwisiot.run();

  if (millis() - previousMillis >= interval) {

    previousMillis = millis();

    // Read PIR Sensor
    pirState = digitalRead(PIR_PIN);

    // Read Reed Switch
    doorState = digitalRead(REED_PIN);

    // PIR Sensor Status
    String motionData;

    if (pirState == HIGH) {

      motionData = "Motion Detected";

    } else {

      motionData = "No Motion";
    }

    // Reed Switch Status
    String doorData;

    if (doorState == LOW) {

      doorData = "Door Closed";

      digitalWrite(LED_PIN, LOW);

    } else {

      doorData = "Door Open";

      digitalWrite(LED_PIN, HIGH);
    }

    // Serial Monitor Output
    Serial.println("--------------------------");

    Serial.print("Motion: ");
    Serial.println(motionData);

    Serial.print("Door: ");
    Serial.println(doorData);

    Serial.print("LED: ");
    Serial.println(doorState == LOW ? "OFF" : "ON");

    // Send data to KiwisIoT
    kiwisiot.send("0", motionData);
    kiwisiot.send("1", doorData);

    Serial.println("Data sent to KiwisIoT!");
  }
}