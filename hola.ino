#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <HX711.h>

#define DHTPIN 5            // Pin de datos del sensor DHT11
#define DHTTYPE DHT11       // Tipo de sensor
#define RELAY1 10           // Pin del primer relé
#define RELAY2 11           // Pin del segundo relé
#define RELAY3 12           // Pin del tercer relé

#define MOTOR 9             // Pin del motor
#define MOTOR_2 8           // Pin del motor 

#define LOADCELL_DOUT_PIN 7 // Pin DOUT del sensor de peso
#define LOADCELL_SCK_PIN 6  // Pin SCK del sensor de peso

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
HX711 scale;

int currentOption = 0;
bool inMenu = true;

int nextButtonPin = 3;      // Pin del botón "Siguiente"
int prevButtonPin = 4;      // Pin del botón "Anterior"
int selectButtonPin = 2;    // Pin del botón "Seleccionar"
int backButtonPin = 13;     // Pin del botón "Volver"

int lastNextButtonState = HIGH; 
int lastPrevButtonState = HIGH;
int lastSelectButtonState = HIGH;
int lastBackButtonState = HIGH;

void setup() {
  lcd.begin(16, 2); // Inicializa el LCD
  lcd.backlight(); // Enciende la luz de fondo del LCD
  dht.begin(); // Inicializa el sensor DHT11
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // Inicializa el sensor de peso
  scale.set_scale(2280.f);                        // Ajusta según la calibración de tu sensor
  scale.tare();            // Ajusta la tara para el sensor de peso
  
  pinMode(nextButtonPin, INPUT_PULLUP); // Configura los pines de los botones como entrada 
  pinMode(prevButtonPin, INPUT_PULLUP);
  pinMode(selectButtonPin, INPUT_PULLUP);
  pinMode(backButtonPin, INPUT_PULLUP);

  pinMode(RELAY1, OUTPUT); // Configura los pines de los relés como salida
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  digitalWrite(RELAY1, HIGH); // Apaga los relés
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  

  pinMode(MOTOR, OUTPUT); // Configura el pin del motor como salida

  displayMenu();
}

void loop() {
  int nextButtonState = digitalRead(nextButtonPin); // Lee el estado de los botones
  int prevButtonState = digitalRead(prevButtonPin);
  int selectButtonState = digitalRead(selectButtonPin);
  int backButtonState = digitalRead(backButtonPin);

  if (nextButtonState == LOW && lastNextButtonState == HIGH && inMenu) { 
    currentOption = (currentOption + 1) % 7;
    displayMenu();
    delay(200);
  }

  if (prevButtonState == LOW && lastPrevButtonState == HIGH && inMenu) {
    currentOption = (currentOption - 1 + 7) % 7;
    displayMenu();
    delay(200);
  }

  if (selectButtonState == LOW && lastSelectButtonState == HIGH) { //
    inMenu = false;
    lcd.clear();
    lcd.print("Option ");
    lcd.print(currentOption + 1);
    lcd.setCursor(0, 1);
    lcd.print("Selected");
    delay(200);

    // Ejecutar acción según la opción seleccionada
    switch (currentOption) {
      case 0:  // Opción 1: Mostrar temperatura y humedad
        displayTemperatureHumidity();
        break;
      case 1:  // Opción 2: Activar primer relé
        testRelay(RELAY1, "Relay 1");
        break;
      case 2:  // Opción 3: Activar segundo relé
        testRelay(RELAY2, "Relay 2");
        break;
      case 3:  // Opción 4: Activar tercer relé
        testRelay(RELAY3, "Relay 3");
        break;
      case 4:  // Opción 5: Medir y mostrar el peso en gramos
        displayWeight();
        break;
      case 5:  // Opción 6: Controlar el motor
        controlMotor();
        break;
      case 6:  // Prender Relé 1 y 2
        digitalWrite(RELAY1, LOW);
        digitalWrite(RELAY2, LOW);
        lcd.clear();
        lcd.print("Relay 1 and 2");
        lcd.setCursor(0, 1);
        lcd.print("ON");
        delay(2000);
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, HIGH);
        lcd.clear();
        lcd.print("Relay 1 and 2");
        lcd.setCursor(0, 1);
        lcd.print("OFF");
        delay(2000);
        inMenu = true;
        displayMenu();
        break;
    }
  }

  if (backButtonState == LOW && lastBackButtonState == HIGH && !inMenu) {
    inMenu = true;
    displayMenu();
    delay(200);
  }

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

void displayTemperatureHumidity() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  lcd.clear();
  if (isnan(temp) || isnan(humidity)) {
    lcd.print("Error reading");
    lcd.setCursor(0, 1);
    lcd.print("sensor data");
  } else {
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
  }
  delay(2000);
  inMenu = true;
  displayMenu();
}

void testRelay(int relayPin, const char* relayName) {
  digitalWrite(relayPin, LOW); // Encender relé
  lcd.clear();
  lcd.print(relayName);
  lcd.setCursor(0, 1);
  lcd.print("ON");
  delay(1000);
  digitalWrite(relayPin, HIGH); // Apagar relé
  lcd.clear();
  lcd.print(relayName);
  lcd.setCursor(0, 1);
  lcd.print("OFF");
  delay(1000);
  inMenu = true;
  displayMenu();
}

void displayWeight() {
  float weight = scale.get_units(1); // Obtiene el peso en gramos 
  
  lcd.clear();
  lcd.print("Weight: ");
  lcd.print(weight, 1); // Muestra el peso con un decimal
  lcd.print(" g");
  delay(2000);
  inMenu = true;
  displayMenu();
}
void controlMotor() {
  lcd.clear();
  lcd.print("Motor Control");
  lcd.setCursor(0, 1);
  lcd.print("Running...");

  digitalWrite(MOTOR_2, HIGH); // Encender motor
  digitalWrite(MOTOR, HIGH); // Encender motor
  delay(2000);
  digitalWrite(MOTOR_2, LOW); // Apagar motor
  digitalWrite(MOTOR, LOW); // Apagar motor
  
  lcd.clear();
  lcd.print("Motor Stopped");
  delay(2000);
  inMenu = true;
  displayMenu();
}
