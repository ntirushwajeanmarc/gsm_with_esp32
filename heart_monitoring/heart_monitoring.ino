#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected from WebSocket");
            break;
        case WStype_CONNECTED:
            Serial.println("Connected to WebSocket");
            break;
        case WStype_TEXT:
            Serial.printf("Received text: %s\n", payload);
            break;
    }
}

void sendSensorData(int sensorValue) {
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["sensor_value"] = sensorValue;
    String jsonStr;
    serializeJson(jsonDoc, jsonStr);
    webSocket.sendTXT(jsonStr);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    
    webSocket.begin("your_server_ip", 8000, "/ws/sensor-data/");
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();

    // Replace this with actual sensor reading
    int sensorValue = analogRead(A0);  // Example sensor reading
    sendSensorData(sensorValue);

    delay(2000);  // Send data every 2 seconds
}
