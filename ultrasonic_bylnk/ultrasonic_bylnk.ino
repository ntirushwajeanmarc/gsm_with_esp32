#define BLYNK_TEMPLATE_ID "TMPL2KdPziaiC"
#define BLYNK_TEMPLATE_NAME "moisture"
#define BLYNK_AUTH_TOKEN "cTYacVsoiYsHiR-WPngGLn9AtZdai_eT"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // I2C LCD library
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define trig D2
#define echo D1
#define relay D5   // Relay connected to D5

long duration;
int distance;
const int waterLevelThreshold = 10;  // Threshold distance in cm to trigger the pump

char auth[] = "cTYacVsoiYsHiR-WPngGLn9AtZdai_eT";
char ssid[] = "mark";
char pass[] = "KCPOiP2N";

BlynkTimer timer;

LiquidCrystal_I2C i2cLcd(0x27, 16, 2);  // LCD I2C address and size (16x2)

void setup()
{
  Wire.begin(21, 22);
  // Pin modes
  pinMode(trig, OUTPUT);  // Trig pin as Output
  pinMode(echo, INPUT);   // Echo pin as Input
  pinMode(relay, OUTPUT); // Relay pin as Output

  // Initial states
  digitalWrite(relay, HIGH);  // Relay OFF (assuming LOW turns the pump ON)

  // Debug and Blynk initialization
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  // LCD initialization
  i2cLcd.init();
  i2cLcd.backlight();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void sendSensor()
{
  // Ultrasonic sensor logic
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;  // Calculating distance in cm

  Serial.print("Distance = ");
  Serial.println(distance);

  Blynk.virtualWrite(V4, distance);
  
  // Display on I2C LCD
  i2cLcd.setCursor(0, 0);
  i2cLcd.print("Distance: " + String(distance) + "cm");

  // Control the pump via relay
  if (distance >= waterLevelThreshold)
  {
    digitalWrite(relay, LOW);  // Turn ON the pump
    i2cLcd.setCursor(0, 1);
    i2cLcd.print("Pump: ON ");
  }
  else
  {
    digitalWrite(relay, HIGH);  // Turn OFF the pump
    i2cLcd.setCursor(0, 1);
    i2cLcd.print("Pump: OFF");
  }
  
  delay(1000);  // Delay before the next reading
}
