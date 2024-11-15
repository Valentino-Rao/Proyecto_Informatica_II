#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <HX711.h>

#define DHTPIN 5            // Pin de datos del sensor DHT11
#define DHTTYPE DHT11       // Tipo de sensor
#define RELAY1 10           // Pin del primer relé
#define RELAY2 11           // Pin del segundo relé
#define RELAY3 12           // Pin del tercer relé
#define RELAY4 9            // Pin del cuarto relé

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

unsigned long startMillis; // Almacena el tiempo de referencia

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
  pinMode(RELAY4, OUTPUT);

  digitalWrite(RELAY1, HIGH); // Apaga los relés
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  

  Serial.begin(9600);
  startMillis = millis(); // Guarda el tiempo actual

  displayMenu();
}

void loop() {
  
  int nextButtonState = digitalRead(nextButtonPin); // Lee el estado de los botones
  int prevButtonState = digitalRead(prevButtonPin);
  int selectButtonState = digitalRead(selectButtonPin);
  int backButtonState = digitalRead(backButtonPin);

   if (nextButtonState == LOW && lastNextButtonState == HIGH && inMenu) { 
    currentOption = (currentOption + 1) % 5;
    displayMenu();
    delay(200);
  }

  if (prevButtonState == LOW && lastPrevButtonState == HIGH && inMenu){
    currentOption = (currentOption - 1 + 5) % 5;
    displayMenu();
    delay(200);
  }

  if (selectButtonState == LOW && lastSelectButtonState == HIGH){
    
    inMenu = false;
    lcd.clear();
    lcd.print("Option ");
    lcd.print(currentOption + 1);
    lcd.setCursor(0, 1);
    lcd.print("Selected");
    delay(200);

    // Switch case para seleccionar la opción

    switch (currentOption){
      case 0:  // Opción 1: Mostrar temperatura y humedad
        displayTemperatureHumidity();
        break;
      case 1:  // Opción 2: Mostrar el peso
        displayPeso();
        break;
      case 2:  // Opción 3: Primera opción de comida, si se selecciona que la cocine
        CocinarOpcion1();
        break;
      case 3:  // Opción 4: Segunda opción de comida, si se selecciona que la cocine
        CocinarOpcion2();
        break;
      case 4:  // Opción 5: Tercera opción de comida, si se selecciona que la cocine
        CocinarOpcion3();
        break;

    }
  }

}

void displayMenu() {
  if (inMenu) {
    lcd.clear();
    lcd.print("Opcion: ");
    lcd.print(currentOption + 1);

    if(currentOption > 1){

      lcd.clear();
      lcd.print("Comida: ");
      lcd.print(currentOption - 1);
      
      if(currentOption == 2){
        lcd.setCursor(0, 1);
        lcd.print("Pollo al Horno");
      }

      if(currentOption == 3){
        lcd.setCursor(0, 1);
        lcd.print("Pescado al Vapor");
      }

      if(currentOption == 4){
        lcd.setCursor(0, 1);
        lcd.print("Pizza Casera");
      }
    }
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
    lcd.print("Humedad: ");
    lcd.print(humidity);
  }
  delay(2000);
  inMenu = true;
  displayMenu();
}

void displayPeso() {
  lcd.clear();
  lcd.print("Peso: ");
  delay(2000);
  float Peso = scale.get_units(1);
  lcd.clear();
  lcd.print(Peso, 1);
  lcd.print(" g");
  delay(2000);
  inMenu = true;
  displayMenu();
}

void CalentarHorno(int temp, float Temp) {
    while(temp < Temp){

    temp= dht.readTemperature();
    
    lcd.clear();
    lcd.print("Calentando Horno");
    delay(1000);
    lcd.clear();
    
    lcd.print("Temp: ");
    lcd.print(temp);
    delay(1000);

    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
  }

  if(temp > Temp){
    lcd.clear();
    lcd.print("Horno Caliente");
    delay(2000);
    digitalWrite(RELAY3, LOW);
    delay(1000);
    digitalWrite(RELAY3, HIGH);
  }
}

void CocinarOpcion1(){
  
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  lcd.clear();
  
  lcd.print("Pesar Pollo: ");
  delay(5000);
  static float pollo = 0;
  pollo = scale.get_units(1);
  lcd.clear();
  lcd.print("Pollo: ");
  lcd.print(pollo, 1);
  lcd.print(" g");
  delay(5000);

  lcd.clear();
  lcd.print("Pesar papas: ");
  delay(5000);
  static float papas = 0;
  papas = scale.get_units(1);
  lcd.clear();
  lcd.print("Papas: ");
  lcd.print(papas, 1);
  lcd.print(" g");
  delay(5000);

  pollo += papas;
  Serial.print("Pollo,");
  Serial.println(pollo);

  lcd.clear();
  lcd.print("MOMENTO PARA");
  lcd.setCursor(0, 1);
  lcd.print("CANCELAR");
  delay(10000);

  if (digitalRead(backButtonPin) == LOW) {
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY4, HIGH);
    lcd.clear();
    lcd.print("Cancelado");
    delay(2000);
    inMenu = true;
    displayMenu();
    return;
  }

  CalentarHorno(temp, 30);   

  delay(5000);
  lcd.clear();
  lcd.print("Cocinando...");
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  startMillis = millis();
  
  int controlluz = 0;
  int controlmotor = 0;
  



while (millis() - startMillis < 6000) {

  float humidity_1 = dht.readHumidity();
  float temp1 = dht.readTemperature();
  
  if(controlluz == 0){
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
  }

  if (controlluz == 1) {
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY1, LOW);
  }

  if (controlmotor == 1) {
    digitalWrite(RELAY4, LOW);
  }

  if (controlmotor == 0) {
    digitalWrite(RELAY4, HIGH);
  }

  lcd.clear();
  delay(100);
  lcd.print("Temp: ");
  lcd.print(temp1);
  lcd.setCursor(0, 1);
  lcd.print("Humedad: ");
  lcd.print(humidity_1);
  lcd.setCursor(0, 1);
  delay(5000);
  lcd.clear();
  lcd.print("Cocinando...");
  delay(1000);

  
  while (temp1 > 40) {
    digitalWrite(RELAY2, HIGH);
    lcd.clear();
    lcd.print("Temperatura");
    lcd.setCursor(0, 1);
    lcd.print("Alta");
    delay(2000);
    controlluz = 1;
    break;
  }

  if (temp1 < 40) {
    controlluz = 0;
  }

  while (humidity_1 < 30) {
    digitalWrite(RELAY4, LOW);
    lcd.clear();
    lcd.print("Humedad");
    lcd.setCursor(0, 1);
    lcd.print("Baja");
    delay(2000);
    controlmotor = 1;
    break;
  }

  if (humidity_1 > 30) {
    controlmotor = 0;
  }

}

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY4, HIGH);
  lcd.clear();
  lcd.print("Pollo Listo");
  digitalWrite(RELAY3, LOW);
  delay(5000);
  digitalWrite(RELAY3, HIGH);
  delay(2000);
  inMenu = true;
  displayMenu();
}

void CocinarOpcion2(){
  lcd.clear();


  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  
  lcd.print("Pesar Pescado: ");
  delay(5000);
  static float pescado = 0;
  pescado = scale.get_units(1);
  lcd.clear();
  lcd.print("Pescado: ");
  lcd.print(pescado, 1);
  lcd.print(" g");
  delay(5000);

  lcd.clear();
  lcd.print("Pesar cebollas: ");
  delay(5000);
  static float cebollas = 0;
  cebollas = scale.get_units(1);
  lcd.clear();
  lcd.print("Cebollas: ");
  lcd.print(cebollas, 1);
  lcd.print(" g");
  delay(5000);

  pescado += cebollas;
  Serial.print("Pescado,");
  Serial.println(pescado);

  lcd.clear();
  lcd.print("MOMENTO PARA");
  lcd.setCursor(0, 1);
  lcd.print("CANCELAR");
  delay(10000);

  if (digitalRead(backButtonPin) == LOW) {
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY4, HIGH);
    lcd.clear();
    lcd.print("Cancelado");
    delay(2000);
    inMenu = true;
    displayMenu();
  return;
  }

  CalentarHorno(temp, 30);


  delay(5000);
  lcd.clear();
  lcd.print("Cocinando...");
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  startMillis = millis();
  
  int controlluz = 0;
  int controlmotor = 0;
  



while (millis() - startMillis < 60000) {

  float humidity_1 = dht.readHumidity();
  float temp1 = dht.readTemperature();
  
  if(controlluz == 0){
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
  }

  if (controlluz == 1) {
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY1, LOW);
  }

  if (controlmotor == 1) {
    digitalWrite(RELAY4, LOW);
  }

  if (controlmotor == 0) {
    digitalWrite(RELAY4, HIGH);
  }

  lcd.clear();
  delay(100);
  lcd.print("Temp: ");
  lcd.print(temp1);
  lcd.setCursor(0, 1);
  lcd.print("Humedad: ");
  lcd.print(humidity_1);
  lcd.setCursor(0, 1);
  delay(5000);
  lcd.clear();
  lcd.print("Cocinando...");
  delay(1000);

  
  while (temp1 > 40) {
    digitalWrite(RELAY2, HIGH);
    lcd.clear();
    lcd.print("Temperatura");
    lcd.setCursor(0, 1);
    lcd.print("Alta");
    delay(2000);
    controlluz = 1;
    break;
  }

  if (temp1 < 40) {
    controlluz = 0;
  }

  while (humidity_1 < 50) {
    digitalWrite(RELAY4, LOW);
    lcd.clear();
    lcd.print("Humedad");
    lcd.setCursor(0, 1);
    lcd.print("Baja");
    delay(2000);
    controlmotor = 1;
    break;
  }

  if (humidity_1 > 50) {
    controlmotor = 0;
  }

}

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY4, HIGH);
  lcd.clear();
  lcd.print("Pescado Listo");
  digitalWrite(RELAY3, LOW);
  delay(5000);
  digitalWrite(RELAY3, HIGH);
  delay(2000);
  inMenu = true;
  displayMenu();
}

 void CocinarOpcion3(){
  lcd.clear();

  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  lcd.print("Pesar Salsa: ");
  delay(5000);
  static float salsa = 0;
  salsa = scale.get_units(1);
  lcd.clear();
  lcd.print("salsa: ");
  lcd.print(salsa, 1);
  lcd.print(" g");
  delay(5000);

  lcd.clear();
  lcd.print("Pesar Masa: ");
  delay(5000);
  static float masa = 0;
  masa = scale.get_units(1);
  lcd.clear();
  lcd.print("Masa: ");
  lcd.print(masa, 1);
  lcd.print(" g");
  delay(5000);

  lcd.clear();
  lcd.print("Pesar Queso: ");
  delay(5000);
  static float queso = 0;
  queso = scale.get_units(1);
  lcd.clear();
  lcd.print("Queso: ");
  lcd.print(queso, 1);
  lcd.print(" g");
  delay(5000);

  float pizza = salsa + masa + queso;

  Serial.print("Pizza,");
  Serial.println(pizza);

  lcd.clear();
  lcd.print("MOMENTO PARA");
  lcd.setCursor(0, 1);
  lcd.print("CANCELAR");
  delay(10000);

  if (digitalRead(backButtonPin) == LOW) {
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY4, HIGH);
    lcd.clear();
    lcd.print("Cancelado");
    delay(2000);
    inMenu = true;
    displayMenu();
  return;
  }
  CalentarHorno(temp, 40);

  delay(5000);
  lcd.clear();
  lcd.print("Cocinando...");
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  startMillis = millis();
  
  int controlluz = 0;
  int controlmotor = 0;
  



while (millis() - startMillis < 60000) {

  float humidity_1 = dht.readHumidity();
  float temp1 = dht.readTemperature();
  
  if(controlluz == 0){
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
  }

  if (controlluz == 1) {
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY1, LOW);
  }

  if (controlmotor == 1) {
    digitalWrite(RELAY4, LOW);
  }

  if (controlmotor == 0) {
    digitalWrite(RELAY4, HIGH);
  }

  lcd.clear();
  delay(100);
  lcd.print("Temp: ");
  lcd.print(temp1);
  lcd.setCursor(0, 1);
  lcd.print("Humedad: ");
  lcd.print(humidity_1);
  lcd.setCursor(0, 1);
  delay(5000);
  lcd.clear();
  lcd.print("Cocinando...");
  delay(1000);

  
  while (temp1 > 60) {
    digitalWrite(RELAY2, HIGH);
    lcd.clear();
    lcd.print("Temperatura");
    lcd.setCursor(0, 1);
    lcd.print("Alta");
    delay(2000);
    controlluz = 1;
    break;
  }

  if (temp1 < 60) {
    controlluz = 0;
  }

  while (humidity_1 < 60) {
    digitalWrite(RELAY4, LOW);
    lcd.clear();
    lcd.print("Humedad");
    lcd.setCursor(0, 1);
    lcd.print("Baja");
    delay(2000);
    controlmotor = 1;
    break;
  }

  if (humidity_1 > 60) {
    controlmotor = 0;
  }

}

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY4, HIGH);
  lcd.clear();
  lcd.print("Pizaa Listo");
  digitalWrite(RELAY3, LOW);
  delay(5000);
  digitalWrite(RELAY3, HIGH);
  delay(2000);
  inMenu = true;
  displayMenu();
}
