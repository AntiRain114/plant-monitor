# ESP8266 IoT Plant Monitoring System

This project is an example of an IoT (Internet of Things) plant monitoring system built using an ESP8266 microcontroller. It allows you to monitor temperature, humidity, and soil moisture levels of a plant and publish the data to an MQTT broker for remote access.

## Hardware Components

- ESP8266 (e.g., NodeMCU or ESP-01)
- DHT22 (AM2302) Temperature and Humidity Sensor
- Soil Moisture Sensor
- Blue LED for Status Indication

## Software Requirements

- Arduino IDE
- Libraries: ESP8266WiFi, ESP8266WebServer, ezTime, PubSubClient, DHT, DHT_U

## Installation and Setup

1. Install the Arduino IDE and required libraries.
2. Connect the hardware components as specified in the code.
Please see the How_to_link_the_board.png to learn how to learn how to connect the hardware components.
<img width="1201" alt="How_to_link_the_board" src="https://github.com/AntiRain114/plant-monitor/assets/92373799/c8824f8a-2eb6-4b2c-aae8-d7a0a2f5a896">

4. Create an `arduino_secrets.h` file with your WiFi credentials and MQTT server information.
5. Upload the code to your ESP8266 using the Arduino IDE.
6. Monitor the ESP8266's serial output for debugging information.

## Code Structure

- `setup()`: Initializes the hardware and connections, sets up MQTT, and syncs time.
- `loop()`: Handles web server requests, reads sensor data, and publishes it to MQTT.
- `readMoisture()`: Reads soil moisture data from the sensor.
- `startWifi()`: Connects to a WiFi network.
- `syncDate()`: Synchronizes the device's time with an NTP server.
- `sendMQTT()`: Publishes temperature, humidity, moisture data, and the situation assessments to MQTT.
- `callback()`: Handles incoming MQTT messages.
- `reconnect()`: Reconnects to the MQTT broker if the connection is lost.
- `startWebserver()`: Sets up an HTTP server for local monitoring.
- `handle_OnConnect()`: Handles web requests and displays sensor data.
- `handle_NotFound()`: Handles 404 errors on the web server.
- `SendHTML()`: Generates an HTML response for the web server with sensor data.

## Accessing Data

You can access the sensor data through the local web server or subscribe to the MQTT topics. Data is published to topics like `student/CASA0014/plant/ucfnzw0/temperature`, `student/CASA0014/plant/ucfnzw0/humidity`, and `student/CASA0014/plant/ucfnzw0/moisture`.

## License

This code is open-source and available under the [MIT License](LICENSE).

## Author

Zekun Wang

Feel free to modify and extend this README to provide more details about your specific project, its purpose, and how to use it.
