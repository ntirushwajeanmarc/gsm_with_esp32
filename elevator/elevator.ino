#include <Keypad.h>
#include <Stepper.h>
#include <TimerOne.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad settings
const byte ROWS = 4;  
const byte COLS = 4;  
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A0, A1, A2, 6};
byte colPins[COLS] = {A3, 3, 4, 2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Stepper motor settings
int stepsPerRevolution = 2048;       
int motspeed = 15;
Stepper myStep(stepsPerRevolution, 8, 10, 9, 11);

// Variables to store the current and target levels
int currentLevel = 1;  // Start at level 1
int targetLevel = 1;

// Function to update motor movement based on the target level
void moveToLevel(int target) {
  int stepCount = (target - currentLevel) * stepsPerRevolution;
  if (stepCount != 0) {
    
    // Determine if the elevator is going up or down
    lcd.clear();
    lcd.setCursor(0, 0);
    if (target > currentLevel) {
      lcd.print("Going UP to:");
    } else {
      lcd.print("Going DOWN to:");
    }
    
    // Show the target level
    lcd.setCursor(0, 1);
    lcd.print("Level ");
    lcd.print(target);
    
    delay(1000);  // Display direction for a moment
    
    myStep.step(stepCount);  // Move the motor
    currentLevel = target;   // Update current level after movement
    lcd.clear();
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  myStep.setSpeed(motspeed);
  Timer1.initialize(5000);
  Timer1.attachInterrupt(level);  // Attach the interrupt to monitor keypresses
  
  lcd.setCursor(6, 0); 
  lcd.print("SMART");
  lcd.setCursor(0, 1); 
  lcd.print("ELEVATOR SYSTEM");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Display current level on LCD
  lcd.setCursor(0, 0);
  lcd.print("Current Level: ");
  lcd.setCursor(14, 0);
  lcd.print(currentLevel);

  // Move to target level if it's different from current level
  if (targetLevel != currentLevel) {
    moveToLevel(targetLevel);
  }
}

// Interrupt function to handle keypresses and set target level
void level() {
  char customKey = customKeypad.getKey();
  if (customKey) {
    // Check which level was pressed
    if (customKey >= '1' && customKey <= '5') {
      Serial.print(customKey);
      targetLevel = customKey - '0';  // Convert char to int
    }
  }
}
