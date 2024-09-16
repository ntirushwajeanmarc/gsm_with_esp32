#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// I2C LCD settings and Fingerprint sensor setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address and LCD dimensions
SoftwareSerial mySerial(2, 3); // Pins for fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define RELAY_PIN       4
#define ACCESS_DELAY    3000 // Unlock duration (milliseconds)
#define BUTTON_PIN      5
#define DEBOUNCE_DELAY  50   // Debounce delay for manual button

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...

  // Initialize the fingerprint sensor
  finger.begin(57600);
  delay(5);

  lcd.init();        // Initialize LCD
  lcd.backlight();   // Turn on LCD backlight

  // Check if fingerprint sensor is connected and working
  if (finger.verifyPassword()) {
    lcd.clear();
    lcd.print("Fingerprint Ready");
    lcd.setCursor(0, 1);
    lcd.print("System Initialized");
    Serial.println("Fingerprint sensor found!");
  } else {
    lcd.clear();
    lcd.print("Fingerprint Error");
    lcd.setCursor(0, 1);
    lcd.print("Check Connection");
    Serial.println("Fingerprint sensor not found");
    while (1) { delay(1); }  // Halt if fingerprint sensor fails
  }

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Initially set relay off (HIGH for low-level trigger)

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Manual unlock button

  // Show system ready message after initialization
  lcd.clear();
  lcd.print("System Ready");
}

void loop() {
  int fingerprintID = getFingerprintIDez(); // Get fingerprint ID

  if (fingerprintID != -1) {
    grantAccess(fingerprintID);
  }

  if (checkManualUnlock()) {
    manualUnlock();
  }

  delay(50); // Small delay before the next cycle
}

// Function to handle fingerprint scanning and return ID
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    // Only show "Access Denied" if a finger was detected but not found
    denyAccess();
    return -1;
  }

  // Found a match, return the fingerprint ID
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

// Function to grant access if fingerprint matches
void grantAccess(int id) {
  lcd.clear();
  lcd.print("Access Granted");
  lcd.setCursor(0, 1);
  lcd.print("Welcome!");

  digitalWrite(RELAY_PIN, LOW); // Unlock door
  delay(ACCESS_DELAY);
  digitalWrite(RELAY_PIN, HIGH); // Lock door again

  lcd.clear();
  lcd.print("System Ready");
}

// Function to deny access if no match is found
void denyAccess() {
  lcd.clear();
  lcd.print("Access Denied");
  delay(2000);  // Delay for denied message display
  lcd.clear();
  lcd.print("System Ready"); // Return to "System Ready" after denial
}

// Check if the manual unlock button is pressed (with debounce)
bool checkManualUnlock() {
  if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
    delay(DEBOUNCE_DELAY); // Debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) return true; // Confirm press
  }
  return false;
}

// Manual unlock process
void manualUnlock() {
  digitalWrite(RELAY_PIN, LOW); // Unlock door
  lcd.clear();
  lcd.print("Manual Unlock");

  // Wait until the button is released
  while (digitalRead(BUTTON_PIN) == LOW);

  digitalWrite(RELAY_PIN, HIGH); // Lock door again
  lcd.clear();
  lcd.print("System Ready");
}