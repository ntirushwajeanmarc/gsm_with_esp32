#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "moisture"
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <HardwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
HardwareSerial gsmSerial(2);

char auth[] = "";
char ssid[] = "mark";
char pass[] = "u8dg48j9";

unsigned long lastSMSTime = 0;
const unsigned long smsInterval = 600000; // 10 minutes in milliseconds
bool smsSent = false;

int sensorVal;
int percentage;
int indicator = 4;
BlynkTimer timer;

void myTimer() {
    sensorVal = analogRead(35);
    percentage = map(sensorVal, 0, 4095, 100, 0);
    Serial.print("Percentage is: ");
    Serial.println(percentage);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moisture: ");
    lcd.print(percentage);
    lcd.print(" %");

    Blynk.virtualWrite(V0, percentage);

    if (percentage >= 38) {
        digitalWrite(indicator, HIGH);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Critical: ");
        lcd.print(percentage);
        lcd.print("%");

        unsigned long currentTime = millis();

        if (!smsSent) { // If SMS has not been sent since the condition was first met
            Serial.println("Threshold hit, attempting to send SMS...");
            sendSMS("Critical alert! Moisture level is very high.");
            lastSMSTime = currentTime;
            smsSent = true;
        } else if (currentTime - lastSMSTime >= smsInterval) {
            Serial.println("Threshold condition persists, sending another SMS...");
            sendSMS("Reminder: Moisture level remains critically high.");
            lastSMSTime = currentTime; // Update the last SMS time
        }
    } else {
        digitalWrite(indicator, LOW);
        smsSent = false; // Reset smsSent if the condition is no longer met
    }
}

void setup() {
    Serial.begin(115200);
    gsmSerial.begin(9600, SERIAL_8N1, 16, 17);
    Wire.begin(21, 22);
    lcd.init();
    lcd.backlight();
    pinMode(indicator, OUTPUT);

    Blynk.begin(auth, ssid, pass);
    lcd.setCursor(0, 0);
    lcd.print("hello gorgeous");

    timer.setInterval(1000L, myTimer);
}

void loop() {
    Blynk.run();
    timer.run();
}

void sendSMS(const char* message) {
    gsmSerial.println("AT");
    delay(500);
    if (gsmSerial.find("OK")) {
        Serial.println("GSM Module is ready");
        gsmSerial.println("AT+CMGF=1");
        delay(500);
        gsmSerial.println("AT+CMGS=\"+250785097557\"");
        delay(500);
        gsmSerial.print(message);
        delay(500);
        gsmSerial.write(26); // CTRL+Z
        delay(500);
        Serial.println("SMS sent successfully.");
    } else {
        Serial.println("Failed to communicate with GSM module.");
    }
}
