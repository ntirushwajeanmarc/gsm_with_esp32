#include <Wire.h>
#include "MAX30102_PulseOximeter.h"
#define BLYNK_TEMPLATE_ID "TMPL2s6kMMXni"
#define BLYNK_TEMPLATE_NAME "HeartMonitoring"
#define BLYNK_AUTH_TOKEN "JQQNVzmVFREuCRbDPjxVyeu_kf9_uJdR"
#define REPORTING_PERIOD_MS 1000

// PulseOximeter offers:
// * beat detection reporting
// * heart rate calculation
// * SpO2 (oxidation level) calculation
PulseOximeter pox;
uint32_t tsLastReport = 0;

// Callback fired when a pulse is detected
void onBeatDetected() {
    Serial.println("Beat detected!");
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    Wire.begin(D2, D1);
    Serial.println("Initializing...");

    // Initialize the PulseOximeter instance
    if (!pox.begin()) {
        Serial.println("Sensor initialization FAILED!");
        while (true); // Halt the program if initialization fails
    } else {
        Serial.println("Sensor initialization SUCCESS!");
    }

    // Reduce IR LED current for power efficiency (7.6mA)
    pox.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);

    // Register callback for beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
    // Continuously update the sensor readings
    pox.update();

    // Periodically report heart rate and SpO2 values
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        float heartRate = pox.getHeartRate();
        float SpO2 = pox.getSpO2();

        // Only display valid readings
        if (heartRate > 0 && SpO2 > 0) {
            Serial.print("Heart rate: ");
            Serial.print(heartRate);
            Serial.print(" bpm / SpO2: ");
            Serial.print(SpO2);
            Serial.println("%");
        } else {
            Serial.println("Waiting for valid readings...");
        }

        // Update timestamp for the next report
        tsLastReport = millis();
    }
}
