#define BLYNK_TEMPLATE_ID "TMPL2KdPziaiC"
#define BLYNK_TEMPLATE_NAME "moisture"
// #define BLYNK_AUTH_TOKEN "k3vUYR27AIbT40mQSmX4I3lntTRiZy9a"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


char auth[] = "cTYacVsoiYsHiR-WPngGLn9AtZdai_eT"; // Enter your Blynk authentication token
char ssid[] ="mark"; // Enter your WiFi SSID
char pass[] = "u8dg48j9";

// Declaring a global variable for sensor data
int sensorVal;

int percentage;

int indicator = D4;


// This function creates the timer object. It's part of the Blynk library
BlynkTimer timer;


void myTimer()
{
  sensorVal = analogRead(A0);
  percentage = map(sensorVal, 0, 1024, 100, 0);
  Serial.print("Percentage is: ");
  Serial.println(percentage);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moistrure: ");
  lcd.print(percentage);
  lcd.print(" %");
  
  Blynk.virtualWrite(V0, percentage);

    if (percentage >= 38) {
    digitalWrite(indicator, HIGH);  // Turn on the LED
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Critical: ");
    lcd.print(percentage);
    lcd.print("%");
    
  } else {
    digitalWrite(indicator, LOW);   // Turn off the LED
  }

}

void setup()
{

  Serial.begin(9600);
  Wire.begin(D2, D1);
  lcd.init();
  lcd.backlight();
  pinMode(indicator,OUTPUT);
  // Connecting to Blynk Cloud
  Blynk.begin(auth, ssid, pass);
  lcd.setCursor(0,0);
  lcd.print("hello gorgous");


  timer.setInterval(1000L, myTimer);
}

void loop()
{

  Blynk.run();
  timer.run();
}