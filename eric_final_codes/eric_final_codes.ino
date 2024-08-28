#include <SoftwareSerial.h>

SoftwareSerial gsm(7, 8);

String received_message = "";
bool message_started = false;

int led0 = 12, led1 = 13, led2 = 2, led3 = 3, led4 = 4, 
    led5 = 5, led6 = 6, led7 = 10, led8 = 11, led9 = 9;

int ledpins[] = {led0, led1, led2, led3, led4, led5, led6, led7, led8, led9};

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  
  gsm.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0"); // Configure to show SMS immediately
  delay(1000);
  
  Serial.println("Setting up LEDs...");
  for (int i = 0; i < sizeof(ledpins) / sizeof(ledpins[0]); i++) {
    pinMode(ledpins[i], OUTPUT);
    Serial.print("LED pin ");
    Serial.print(ledpins[i]);
    Serial.println(" set as OUTPUT");
    digitalWrite(ledpins[i], LOW);
  }
  Serial.println("Setup complete. Waiting for SMS...");
}

void loop() {
  while (gsm.available()) {
    char c = gsm.read();
    Serial.print(c);  // Echo all characters to Serial for debugging

    if (c == '\n' && message_started) {
      Serial.println("\nEnd of message detected. Processing...");
      processMessage(received_message);
      received_message = "";
      message_started = false;
    } else if (message_started) {
      received_message += c;
    } else if (received_message.startsWith("+CMT:")) {
      if (c == '\n') {
        message_started = true;
        Serial.println("\nMessage content starts on next line.");
      }
    } else {
      received_message += c;
      if (received_message.endsWith("\r\n")) {
        received_message = "";
      }
    }
  }
}

void processMessage(String message) {
  Serial.println("Processing message: " + message);
  
  // Turn off all LEDs initially
  for (int i = 0; i < sizeof(ledpins) / sizeof(ledpins[0]); i++) {
    digitalWrite(ledpins[i], LOW);
  }
  
  // Iterate through the message
  for (int i = 0; i < message.length(); i++) {
    char c = message[i];
    if (isDigit(c)) {
      int ledIndex = c - '0'; // Convert char to corresponding ledpins index (0-9)
      if (ledIndex >= 0 && ledIndex < sizeof(ledpins) / sizeof(ledpins[0])) {
        Serial.print("Turning on LED pin ");
        Serial.println(ledpins[ledIndex]);
        digitalWrite(ledpins[ledIndex], HIGH);
        delay(200); // Increased delay for visibility
        digitalWrite(ledpins[ledIndex], LOW);
        delay(200); // Increased delay between LED activations
      }
    }
  }
  Serial.println("Message processing complete.");
}
