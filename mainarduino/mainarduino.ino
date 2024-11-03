#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the library with the I2C address and dimensions of the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define button pins
const int nextButtonPin = 2;
const int prevButtonPin = 3;
const int selectButtonPin = 4;
const int backButtonPin = 5;

// Variables to keep track of the current menu and selection
int currentOption = 0;
const int totalOptions = 10;
bool inMenu = true;

// Variables to store the previous state of the buttons
bool lastNextButtonState = LOW;
bool lastPrevButtonState = LOW;
bool lastSelectButtonState = LOW;
bool lastBackButtonState = LOW;

void setup() {
  // Set up the LCD's number of columns and rows
  lcd.begin(16,2);
  lcd.backlight();
  // Print the initial menu
  displayMenu();

  // Set up the button pins as input
  pinMode(nextButtonPin, INPUT_PULLUP);
  pinMode(prevButtonPin, INPUT_PULLUP);
  pinMode(selectButtonPin, INPUT_PULLUP);
  pinMode(backButtonPin, INPUT_PULLUP);
}

void loop() {
  // Read the current state of the buttons
  bool nextButtonState = digitalRead(nextButtonPin);
  bool prevButtonState = digitalRead(prevButtonPin);
  bool selectButtonState = digitalRead(selectButtonPin);
  bool backButtonState = digitalRead(backButtonPin);

  // Check if the next button is pressed
  if (nextButtonState == LOW && lastNextButtonState == HIGH) {
    currentOption = (currentOption + 1) % totalOptions;
    displayMenu();
    delay(200); // Debounce delay
  }

  // Check if the previous button is pressed
  if (prevButtonState == LOW && lastPrevButtonState == HIGH) {
    currentOption = (currentOption - 1 + totalOptions) % totalOptions;
    displayMenu();
    delay(200); // Debounce delay
  }

  // Check if the select button is pressed
  if (selectButtonState == LOW && lastSelectButtonState == HIGH) {
    inMenu = false;
    lcd.clear();
    lcd.print("Option ");
    lcd.print(currentOption + 1);
    lcd.setCursor(0, 1);
    lcd.print("Selected");
    delay(200); // Debounce delay
  }

  // Check if the back button is pressed
  if (backButtonState == LOW && lastBackButtonState == HIGH && !inMenu) {
    inMenu = true;
    displayMenu();
    delay(200); // Debounce delay
  }

  // Update the last button states
  lastNextButtonState = nextButtonState;
  lastPrevButtonState = prevButtonState;
  lastSelectButtonState = selectButtonState;
  lastBackButtonState = backButtonState;
}

void displayMenu() {
  if (inMenu) {
    lcd.clear();
    lcd.print("Option ");
    lcd.print(currentOption + 1);
  }
}
