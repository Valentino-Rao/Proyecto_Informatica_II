// This code has to control
// a)one resistor that will heat up an oven.
// b)one temperature sensor will be used to control the temperature of the oven and the temperature of the ingredients.
// c)one button that will be used to turn on and off the oven.
// d)a buzzer that will sound when the oven the recipe is ready. 
// e)a LED that will turn on when the oven is on.
// f)one humidity sensor that will be used to control the humidity of the oven.
// g)one humidifier that will be used to control the humidity of the oven
// h)one weight sensor that will be used to control the weight of the ingredients.
// i)one way to select a food, display instructions; e.g: "Select the food: 1)Pizza 2)Cake 3)Cookies 4)Bread 5)meat with vegetables 6)chicken with vegetables" and others.
// j)The arduino will have to display the temperature, humidity, and the weight of the ingredients.
// k)The arduino will have to display the time left for the recipe to be ready.
// l)The arduino will have to display the ingredients that are needed for the recipe.The arduino will have to display very simple instructions for the recipe.
// m)The arduino will have to display very simple instructions for the recipe.
// n)The arduino will have to display the temperature and humidity of the oven.
// o)The arduino will have to transfer the data to a computer, by USB using the UART protocol, this data will be stored in a file and will be used to analyze, in a c++ program, the macro and micro nutrients of the recipe.
// explain every line in spanish

#include <Wire.h> // Include the Wire library for I2C communication
#include "HX711.h" // Include the HX711 library for the weight sensor

#define WEIGHT_SENSOR_DOUT_PIN 6 // Define the pin that the weight sensor data output is connected to
#define WEIGHT_SENSOR_SCK_PIN 7 // Define the pin that the weight sensor clock is connected to

HX711 scale; // Create an instance of the HX711 class

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  scale.begin(WEIGHT_SENSOR_DOUT_PIN, WEIGHT_SENSOR_SCK_PIN); // Initialize the weight sensor
  lcd.begin(16, 2); // Initialize the LCD screen with 16 columns and 2 rows
  pinMode(TEMP_SENSOR_PIN, INPUT); // Set the temperature sensor pin as an input
  pinMode(HUMIDITY_SENSOR_PIN, INPUT); // Set the humidity sensor pin as an input
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as an input with a pull-up resistor
  pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as an output
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
  lcd.print("Oven Controller"); // Print a message on the LCD screen
  delay(2000); // Wait for 2 seconds
  lcd.clear(); // Clear the LCD screen
}

void loop() {
  int tempValue = analogRead(TEMP_SENSOR_PIN); // Read the temperature sensor value
  float temperature = (tempValue / 1024.0) * 500.0; // Convert to temperature to Celsius

  int humidityValue = analogRead(HUMIDITY_SENSOR_PIN); // Read the humidity sensor value
  float humidity = (humidityValue / 1024.0) * 100.0; // Convert to humidity percentage

  float weight = scale.get_units(10); // Read the weight sensor value

  if (digitalRead(BUTTON_PIN) == LOW) {
    ovenOn = !ovenOn; // Toggle the oven state
    delay(500); // Debounce delay
  }

  if (ovenOn) {
    //select the food
    lcd.setCursor(0, 0); // Set the cursor to the first row
    lcd.print("Select the food:"); // Print the food selection message
    lcd.setCursor(0, 1); // Set the cursor to the second row
    lcd.print("1)Pizza 2)Cake"); // Print the food options
    lcd.setCursor(0, 2); // Set the cursor to the third row
    lcd.print("3)Cookies 4)Bread"); // Print the food options
    lcd.setCursor(0, 3); // Set the cursor to the fourth row
    lcd.print("5)Meat 6)Chicken"); // Print the food options

    int foodSelection = 0; // Variable to store the selected food
    while (foodSelection == 0) {
      if (Serial.available() > 0) { // Check if there is data available on the serial port
      char input = Serial.read(); // Read the input from the serial monitor
      switch (input) { // Check the input value
        
        case '1':
        foodSelection = 1;
        lcd.clear();
        lcd.print("Pizza selected");
        //display the ingredients
        lcd.setCursor(0, 0);
        lcd.print("Ingredients:");
        lcd.setCursor(0, 1);
        lcd.print("Dough, Cheese,");
        lcd.setCursor(0, 2);
        lcd.print("Tomato Sauce,");
        lcd.setCursor(0, 3);
        lcd.print("basil");
        //measure the ingredients
        lcd.setCursor(0, 0);
        lcd.print("Measure the ingredients:");
        lcd.setCursor(0, 1);
        lcd.print("Dough: 500g");
        lcd.setCursor(0, 2);
        lcd.print("Cheese: 200g");
        lcd.setCursor(0, 3);
        lcd.print("Tomato Sauce: 100g");
        //display the instructions
        lcd.setCursor(0, 0);
        lcd.print("Instructions:");
        lcd.setCursor(0, 1);
        lcd.print("1. Preheat oven to 200C");
        lcd.setCursor(0, 2);
        lcd.print("2. Roll out dough");
        lcd.setCursor(0, 3);
        lcd.print("3. Add sauce and cheese");
        break;
        
        case '2':
        foodSelection = 2;
        lcd.clear();
        lcd.print("Cake selected");
        break;
        
        case '3':
        foodSelection = 3;
        lcd.clear();
        lcd.print("Cookies selected");
        break;
        
        case '4':
        foodSelection = 4;
        lcd.clear();
        lcd.print("Bread selected");
        break;
        
        case '5':
        foodSelection = 5;
        lcd.clear();
        lcd.print("Meat selected");
        break;
        
        case '6':
        foodSelection = 6;
        lcd.clear();
        lcd.print("Chicken selected");
        break;
        
        default:
        lcd.clear();
        lcd.print("Invalid selection");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Select the food:");
        lcd.setCursor(0, 1);
        lcd.print("1)Pizza 2)Cake");
        lcd.setCursor(0, 2);
        lcd.print("3)Cookies 4)Bread");
        lcd.setCursor(0, 3);
        lcd.print("5)Meat 6)Chicken");
        break;
      }
      }
    }

    digitalWrite(LED_PIN, HIGH); // Turn on the LED indicator
    if (temperature < targetTemp) {
      digitalWrite(9, HIGH); // Turn on heating element
    } else {
      digitalWrite(9, LOW); // Turn off heating element
    }
    if (humidity < targetHumidity) {
      digitalWrite(8, HIGH); // Turn on humidifier
    } else {
      digitalWrite(8, LOW); // Turn off humidifier
    }
  } else {
    digitalWrite(LED_PIN, LOW); // Turn off the LED indicator
    digitalWrite(9, LOW); // Turn off heating element
    digitalWrite(8, LOW); // Turn off humidifier
  }

  lcd.setCursor(0, 0); // Set the cursor to the first row
  lcd.print("Temp: "); // Print the temperature label
  lcd.print(temperature); // Print the temperature value
  lcd.print(" C"); // Print the temperature unit

  lcd.setCursor(0, 1); // Set the cursor to the second row
  lcd.print("Humidity: "); // Print the humidity label
  lcd.print(humidity); // Print the humidity value
  lcd.print(" %"); // Print the humidity unit

  lcd.setCursor(0, 2); // Set the cursor to the third row
  lcd.print("Weight: "); // Print the weight label
  lcd.print(weight); // Print the weight value
  lcd.print(" g"); // Print the weight unit

  if (temperature >= targetTemp) {
    digitalWrite(BUZZER_PIN, HIGH); // Sound buzzer
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
  }

  // Send data to the computer via UART
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Weight: ");
  Serial.print(weight);
  Serial.println(" g");

  delay(1000); // Update every second
}
