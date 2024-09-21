#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "mark";
const char* password = "KCPOiP2N";

// WebSocket client
WebSocketsClient webSocket;

// WebSocket event handling
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected from WebSocket");
            break;
        case WStype_CONNECTED:
            Serial.println("Connected to WebSocket");
            // You can also send an initial message once connected, like:
            webSocket.sendTXT("ESP32 connected");
            break;
        case WStype_TEXT:
            Serial.printf("Received text: %s\n", payload);
            break;
                case WStype_ERROR:
            Serial.println("WebSocket Error");
            break;
        case WStype_BIN:
            Serial.println("Received binary data");
            break;
        default:
            Serial.printf("Unhandled event type: %d\n", type);
            break;
    }
}

// Send sensor data to WebSocket
void sendSensorData(int sensorValue) {
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["sensor_value"] = sensorValue;
    String jsonStr;
    serializeJson(jsonDoc, jsonStr);
    webSocket.sendTXT(jsonStr);
}

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Initialize WebSocket
    Serial.println("Connecting to WebSocket...");
    Serial.print("Host: ");
    Serial.println("k4z55dtn-8000.uks1.devtunnels.ms");
    Serial.print("Port: ");
    Serial.println(8000);
    Serial.print("URL: ");
    Serial.println("/ws/sensor-data/");
    
    webSocket.begin("k4z55dtn-8000.uks1.devtunnels.ms/", 8000, "/ws/sensor-data/");

    // Optional: Enable heartbeat to maintain connection
    webSocket.enableHeartbeat(15000, 3000, 2);

    // Set WebSocket event handling function
    webSocket.onEvent(webSocketEvent);

    Serial.println("Setup complete. Entering main loop...");
}

void loop() {
    webSocket.loop();

    // Replace with actual sensor reading on ESP32, for example using GPIO 34
    int sensorValue = analogRead(34);  // Example for an analog sensor on GPIO 34
    sendSensorData(sensorValue);

    delay(2000);  // Send data every 2 seconds
}