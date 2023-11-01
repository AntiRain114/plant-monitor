
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ezTime.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DHT_U.h>

//#include <ESP8266HTTPClient.h>  developing function

#define DHTTYPE DHT22   // Define the DHT sensor type as DHT22 (AM2302) or AM2321

// Sensors - DHT22 and Nails
uint8_t DHTPin = 12;         // Connect the DHT22 sensor to Pin 12 on the Huzzah
uint8_t soilPin = 0;      // Use ADC or A0 pin on the Huzzah for the soil moisture sensor
float Temperature;
float Humidity;
int Moisture = 1; // Initialize Moisture with a default value (in case web page loads before readMoisture is called)
int sensorVCC = 13; // Pin to control power to the soil moisture sensor
int blueLED = 2;  // Pin for a blue LED status indicator
DHT dht(DHTPin, DHTTYPE);   // Initialize the DHT sensor.



// Wifi and MQTT
#include "arduino_secrets.h"   //import password file


const char* ssid     = SECRET_SSID; // Set the WiFi SSID from the arduino_secrets.h file
const char* password = SECRET_PASS; // Set the WiFi password from the arduino_secrets.h file
const char* mqttuser = SECRET_MQTTUSER; // Set the MQTT username from the arduino_secrets.h file
const char* mqttpass = SECRET_MQTTPASS; // Set the MQTT password from the arduino_secrets.h file
//const char* discordWebhookUrl = "https://discord.com/api/webhooks/1169048071548702750/pDUWnAErtUo4E1O-18NsnlrrHvUGMFfTbjV3VCIs01vVf7OmUspYtW-qxR5xg6zPU-IY";
//developing function this is the webhook that will be used

ESP8266WebServer server(80);
const char* mqtt_server = "mqtt.cetools.org"; // MQTT server address
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// Date and time
Timezone GB;



void setup() {
  // Set up LED to be controllable via broker
  // Initialize the BUILTIN_LED pin as an output
  // Turn the LED off by making the voltage HIGH
  pinMode(BUILTIN_LED, OUTPUT);     
  digitalWrite(BUILTIN_LED, HIGH);  

 // Set up the outputs to control the soil sensor switch and the blue LED for status indicator
  pinMode(sensorVCC, OUTPUT); 
  digitalWrite(sensorVCC, LOW);
  pinMode(blueLED, OUTPUT); 
  digitalWrite(blueLED, HIGH);

 // Open a serial connection for debug information
  Serial.begin(115200);
  delay(100);

  // start DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();

  // Run WIFI and internet initialization functions
  startWifi();
  startWebserver();
  syncDate();

  // Connect to the MQTT server
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);

}

void loop() {
  // Handle incoming requests to the web server
  server.handleClient();

  if (minuteChanged()) {
    readMoisture();
    sendMQTT();
    Serial.println(GB.dateTime("H:i:s")); // Print the current time in UTC
  }
  
  client.loop();
}

void readMoisture(){
  
  // Power the soil moisture sensor
  digitalWrite(sensorVCC, HIGH);
  digitalWrite(blueLED, LOW);
  delay(100);
  // Read the data from the sensor
  
  Moisture = analogRead(soilPin);   
  //Moisture = map(analogRead(soilPin), 0,xxx, 0, 100); Haven't get the real maximum value of the sensor, will be changed later.
  digitalWrite(sensorVCC, LOW);  
  digitalWrite(blueLED, HIGH);
  delay(100);
  Serial.print("Wet ");
  Serial.println(Moisture);   // read the data from the nails
}

void startWifi() {
 // Connect to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

// Check if connected and wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void syncDate() {
  // Get the real date and time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());

}

void sendMQTT() {
// Set the MQTT server reconnect method
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Temperature = dht.readTemperature(); // Gets the temperature data
  snprintf (msg, 50, "%.1f", Temperature);  //data format
  Serial.print("Publish message for t: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnzw0/temperature", msg); //data publish path

   // Check if temperature exceeds a predefined threshold
  float TEMPERATURE_UPPER_THRESHOLD = 30.0;
  float TEMPERATURE_LOWER_THRESHOLD = 15.0;
  bool temperatureAboveUpperThreshold = (Temperature > TEMPERATURE_UPPER_THRESHOLD);
  bool temperatureBelowLowerThreshold = (Temperature < TEMPERATURE_LOWER_THRESHOLD);

  if (temperatureAboveUpperThreshold) {
    client.publish("student/CASA0014/plant/ucfnzw0/temperature_status", "Temperature exceeded upper threshold. Take action!");
  } else if (temperatureBelowLowerThreshold) {
    client.publish("student/CASA0014/plant/ucfnzw0/temperature_status", "Temperature below lower threshold. Take action!");
  } else {
    client.publish("student/CASA0014/plant/ucfnzw0/temperature_status", "Temperature within normal range.");
  }

  Humidity = dht.readHumidity(); // Gets the  humidity data
  snprintf (msg, 50, "%.0f", Humidity); //data format
  Serial.print("Publish message for h: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnzw0/humidity", msg); //data publish path

  // Check if humidity exceeds a predefined threshold
  float HUMIDITY_UPPER_THRESHOLD = 60;
  float HUMIDITY_LOWER_THRESHOLD = 40;


  bool humidityAboveUpperThreshold = (Humidity > HUMIDITY_UPPER_THRESHOLD);
  bool humidityBelowLowerThreshold = (Humidity < HUMIDITY_LOWER_THRESHOLD);

  if (humidityAboveUpperThreshold) {
    client.publish("student/CASA0014/plant/ucfnzw0/humidity_status", "Humidity exceeded upper threshold. Take action!");
  } else if (humidityBelowLowerThreshold) {
    client.publish("student/CASA0014/plant/ucfnzw0/humidity_status", "Humidity below lower threshold. Take action!");
  } else {
    client.publish("student/CASA0014/plant/ucfnzw0/humidity_status", "Humidity within normal range.");
  }
  //Moisture = analogRead(soilPin);   // moisture read by readMoisture function
  snprintf (msg, 50, "%.0i", Moisture); //data format
  Serial.print("Publish message for m: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnzw0/moisture", msg); //data publish path

   // Check if soil moisture exceeds a predefined threshold
  float MOISTURE_UPPER_THRESHOLD = 40;
  float MOISTURE_LOWER_THRESHOLD = 20;

  bool moistureAboveUpperThreshold = (Moisture > MOISTURE_UPPER_THRESHOLD);
  bool moistureBelowLowerThreshold = (Moisture < MOISTURE_LOWER_THRESHOLD);

  if (moistureAboveUpperThreshold) {
    client.publish("student/CASA0014/plant/ucfnzw0/moisture_status", "Moisture exceeded upper threshold. Take action!");
  } else if (moistureBelowLowerThreshold) {
    client.publish("student/CASA0014/plant/ucfnzw0/moisture_status", "Moisture below lower threshold. Take action!");
  } else {
    client.publish("student/CASA0014/plant/ucfnzw0/moisture_status", "Moisture within normal range.");
  }

}

void callback(char* topic, byte* payload, unsigned int length) {
  // Display the received message's topic
  // show the message 
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  // Iterate through the bytes of the message and print them
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Turn on the LED if '1' was received as the first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);  // Turn the LED on
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
  }

}

void reconnect() {
  // Loop until connected to the MQTT
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Use clientID, username and password to 
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("student/CASA0014/plant/ucfnzw0/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void startWebserver() {
  // when connected and IP address obtained start HTTP server  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");  
}

void handle_OnConnect() {
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  server.send(200, "text/html", SendHTML(Temperature, Humidity, Moisture));
}

void handle_NotFound() {
   // Send a 404 Not Found response with a plain text message
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat, float Humiditystat, int Moisturestat) {
   // Send a 404 Not Found response with a plain text message
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP8266 DHT22 Report</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP8266 Huzzah DHT22 Report</h1>\n";

 // Insert temperature, humidity, and moisture data into the HTML
  ptr += "<p>Temperature: ";
  ptr += (int)Temperaturestat;
  ptr += " C</p>";
  ptr += "<p>Humidity: ";
  ptr += (int)Humiditystat;
  ptr += "%</p>";
  ptr += "<p>Moisture: ";
  ptr += Moisturestat;
  ptr += "</p>";
  ptr += "<p>Sampled on: ";
  ptr += GB.dateTime("l,");
  ptr += "<br>";
  ptr += GB.dateTime("d-M-y H:i:s T");
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
   // Return the HTML page as a string
  return ptr;
}
/* Developing function
void sendStatusToDiscordWebhook(String statusMessage) {
  HTTPClient http;

  // Configure HTTP request
  http.begin(discordWebhookUrl);
  http.addHeader("Content-Type", "application/json");

  // Create JSON message
  String jsonMessage = "{\"content\": \"" + statusMessage + "\"}";

  // Send POST request
  int httpResponseCode = http.POST(jsonMessage);

  if (httpResponseCode == 204) {
    Serial.println("Status sent to Discord Webhook successfully.");
  } else {
    Serial.print("Error sending status to Discord Webhook. HTTP error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
*/
