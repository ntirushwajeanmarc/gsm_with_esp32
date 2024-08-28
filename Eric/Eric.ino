#include <SoftwareSerial.h> 
SoftwareSerial gsm(7, 8); 

char incoming_char; 
String received_message = ""; 
bool message_received = false;

int led0 = 12;
int led1 = 13;
int led2 = 2;
int led3 = 3;
int led4 = 4;
int led5 = 5;
int led6 = 6;
int led7 = 10;
int led8 = 11;
int led9 = 9;

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
    digitalWrite(ledpins[i],LOW);
  }
}

void loop(){
  if (gsm.available()) { 
    incoming_char = gsm.read();
    Serial.print(incoming_char);
    }
    if (incoming_char == '\r' || incoming_char == '\n') { 
    if (received_message.length() > 0) {
        received_message += incoming_char;
        message_received = true; // Set flag to indicate message has been received
        processMessage(received_message);
        received_message = ""; // Clear message after processing
        message_received =message_received;
    }}}
void processMessage(String message) {
  // Turn off all LEDs initially
  for (int i = 0; i < sizeof(ledpins) / sizeof(ledpins[0]); i++) {
    digitalWrite(ledpins[i], LOW);
  }
  
  // Iterate through the message
  for (int i = 0; i < message.length(); i++) {
    char c = message[i];
    Serial.println(c);
    int ledIndex = c - '0'; // Convert char to corresponding ledpins index (0-9)
    if (ledIndex >= 0 && ledIndex < sizeof(ledpins) / sizeof(ledpins[0])) {
      Serial.print("Turning on LED pin ");
      Serial.println(ledpins[ledIndex]);
      digitalWrite(ledpins[ledIndex], HIGH);
      delay(100); // Adjust delay as needed
      digitalWrite(ledpins[ledIndex], LOW);
      delay(100);
    }
  }
}
