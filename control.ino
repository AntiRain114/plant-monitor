#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WIFI&PASSWORD
#include "arduino_secrets.h"   //import password file


const char* ssid     = SECRET_SSID; // Set the WiFi SSID from the arduino_secrets.h file
const char* password = SECRET_PASS; // Set the WiFi password from the arduino_secrets.h file


// PORT
ESP8266WebServer server(80);

// PIN
const int relayPin = 5; // 

unsigned long wateringTime = 10000; // WATERING TIME
unsigned long startWateringTime = 0; // START TIME

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); //SHUT AT BEGINNING

  server.on("/water", handleWaterCommand);
  server.begin();
}

void loop() {
  server.handleClient();

  // CHECK THE END POINT
  if (startWateringTime != 0 && millis() - startWateringTime >= wateringTime) {
    digitalWrite(relayPin, LOW); // STOP WSTERING
    startWateringTime = 0; // RESET TIME
  }
}

void handleWaterCommand() {
  String action = server.arg("action");
  if (action == "on") {
    digitalWrite(relayPin, HIGH); // TURN ON THE RELAY
    startWateringTime = millis(); // RECORD THE WATER TIME
    server.send(200, "text/plain", "Water pump turned ON for " + String(wateringTime / 1000) + " seconds");
  } else if (action == "off") {
    digitalWrite(relayPin, LOW); // SHUT THE RELAY
    startWateringTime = 0; // RESET THE TIME
    server.send(200, "text/plain", "Water pump turned OFF");
  } else {
    server.send(400, "text/plain", "Invalid command");
  }
}
