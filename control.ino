#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// 替换为您的WiFi网络名称和密码
#include "arduino_secrets.h"   //import password file


const char* ssid     = SECRET_SSID; // Set the WiFi SSID from the arduino_secrets.h file
const char* password = SECRET_PASS; // Set the WiFi password from the arduino_secrets.h file


// ESP8266 Web服务器在端口80上运行
ESP8266WebServer server(80);

// 定义连接到继电器的GPIO引脚
const int relayPin = 5; // 例如使用D1引脚

unsigned long wateringTime = 10000; // 浇水时间，单位为毫秒（例如10000毫秒即10秒）
unsigned long startWateringTime = 0; // 开始浇水的时间

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
  digitalWrite(relayPin, LOW); // 默认关闭继电器

  server.on("/water", handleWaterCommand);
  server.begin();
}

void loop() {
  server.handleClient();

  // 检查是否到达浇水时间的结束点
  if (startWateringTime != 0 && millis() - startWateringTime >= wateringTime) {
    digitalWrite(relayPin, LOW); // 关闭继电器，停止浇水
    startWateringTime = 0; // 重置开始浇水的时间
  }
}

void handleWaterCommand() {
  String action = server.arg("action");
  if (action == "on") {
    digitalWrite(relayPin, HIGH); // 打开继电器
    startWateringTime = millis(); // 记录开始浇水的时间
    server.send(200, "text/plain", "Water pump turned ON for " + String(wateringTime / 1000) + " seconds");
  } else if (action == "off") {
    digitalWrite(relayPin, LOW); // 手动关闭继电器
    startWateringTime = 0; // 重置开始浇水的时间
    server.send(200, "text/plain", "Water pump turned OFF");
  } else {
    server.send(400, "text/plain", "Invalid command");
  }
}