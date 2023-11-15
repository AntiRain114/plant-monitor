# ESP8266 IoT Plant Monitoring System

## Project Overview

This project presents an IoT (Internet of Things) plant monitoring system built using an ESP8266 microcontroller. The system is designed to monitor key environmental factors affecting plant health - specifically temperature, humidity, and soil moisture levels. This data is crucial for the proper care and maintenance of plants, particularly those with specific climate needs, like Rosemary. The gathered data is then published to an MQTT broker, allowing for remote access and monitoring.

## Hardware Components

-ESP8266: The core microcontroller for the system.
-DHT22 (AM2302) Temperature and Humidity Sensor: Measures ambient temperature and humidity.
-Soil Moisture Sensor (with nails): Determines the moisture level of the plant’s soil.
-Blue LED: Serves as an on-board status indicator.

## Software Requirements

- Arduino IDE: The development environment used for programming the ESP8266.
- Libraries: ESP8266WiFi,
-- ESP8266WebServer,
-- ezTime,
-- PubSubClient,
-- DHT,
-- DHT_U,
-- ESP8266HTTPClient


## Plant Introduction: Rosemary

- Rosemary (scientific name: Rosmarinus officinalis) is a common herb known for its aromatic and culinary uses. It is a versatile plant loved by many chefs and gardening enthusiasts.

- Optimal Temperature:

- Rosemary thrives in warm climates. The optimal temperature range for its growth is typically between 15°C to 30°C (59°F to 86°F). Within this range, it can grow vigorously and remain healthy.
Optimal Air Humidity:

- Rosemary generally adapts well to low humidity environments. It can thrive in relatively dry air conditions. Typical indoor humidity (relative humidity) ranging from 40% to 60% is usually suitable.
Optimal Soil Moisture:

- Rosemary requires well-draining soil, which means the soil should be kept moderately moist but not overly saturated. It's recommended to wait for the top layer of the soil to dry out before watering again to ensure the soil doesn't become too wet.
Rosemary is a relatively easy-to-care-for plant and is usually known to prefer abundant sunlight and well-ventilated growth conditions. This multifunctional herb is not only used for culinary purposes but also finds applications in medicinal and decorative settings. By managing the right temperature, humidity, and soil moisture, you can successfully cultivate rosemary in your home or garden, enjoying its unique aroma and versatility.

## Installation and Setup

1. Arduino IDE and Libraries: Install the Arduino IDE and the above-mentioned libraries.
2. Hardware Assembly: Connect the DHT22, soil moisture sensor, and LED to the ESP8266 as per the wiring diagram. Refer to How_to_link_the_board.png in the repository for guidance.
<img width="1201" alt="How_to_link_the_board" src="https://github.com/AntiRain114/plant-monitor/assets/92373799/c8824f8a-2eb6-4b2c-aae8-d7a0a2f5a896">

3. Configuration File: Create an arduino_secrets.h file with your WiFi and MQTT broker credentials
4. Code Upload: Using the Arduino IDE, upload the provided code to the ESP8266.
5. Debugging: Monitor the serial output of the ESP8266 for debugging information.

## Code Structure

-setup(): Sets up the sensors, WiFi, MQTT, and synchronizes time.
-loop(): Main loop handling web requests, sensor readings, and MQTT publishing.
-Sensor Readings: Temperature, humidity, and soil moisture are read and processed.
-MQTT Integration: Sensor data is published to specific MQTT topics.
-Web Server: A local web server provides real-time data access.
-Alert System: The system sends alerts if readings are beyond optimal thresholds.

## Accessing Data

Data is accessible via the local web server or by subscribing to MQTT topics like student/CASA0014/plant/ucfnzw0/temperature. Future developments include forwarding data to cloud servers and alerting via Discord bots.

## Future plan
Integration with cloud services for data storage and analysis.
Development of mobile and desktop notifications through Webhooks or APIs.
Implementing AI for predictive analysis of plant health.



## License

This code is open-source.

## Author

Zekun Wang
