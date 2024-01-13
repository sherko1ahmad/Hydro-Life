#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define WATER_PUMP_RELAY_PIN 12    // Relay pin for the water pump
#define FAN_RELAY_PIN 13           // Relay pin for the fan

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 is the I2C address, adjust as needed

unsigned long previousMillis = 0;
const long interval = 3600000; // Set the interval to 1 hour (in milliseconds)
const int fanDuration = 300000; // Set the fan duration to 5 minutes (in milliseconds)

void setup() {
  lcd.init();                      // Initialize LCD
  lcd.backlight();                 // Turn on backlight
  lcd.clear();                     // Clear the LCD screen
  Serial.begin(9600);              // Initialize Serial communication
  dht.begin();                     // Initialize DHT sensor
  pinMode(WATER_PUMP_RELAY_PIN, OUTPUT);  // Set the water pump relay pin as an OUTPUT
  pinMode(FAN_RELAY_PIN, OUTPUT);        // Set the fan relay pin as an OUTPUT
  digitalWrite(WATER_PUMP_RELAY_PIN, LOW); // Initially turn off the water pump
  digitalWrite(FAN_RELAY_PIN, LOW);       // Initially turn off the fan
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to turn on the fans
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the last time the fans were turned on

    // Turn on the fan relay for the specified duration
    digitalWrite(FAN_RELAY_PIN, HIGH);
    delay(fanDuration);
    digitalWrite(FAN_RELAY_PIN, LOW);
  }

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Print to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" Â°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  // Control the water pump with the relay based on humidity
  if (humidity < 50.0) {
    digitalWrite(WATER_PUMP_RELAY_PIN, HIGH); // Turn on the water pump relay
  } else {
    digitalWrite(WATER_PUMP_RELAY_PIN, LOW);  // Turn off the water pump relay
  }

  // Control the fan with the relay based on temperature
  if (temperature > 22.0) {
    digitalWrite(FAN_RELAY_PIN, HIGH); // Turn on the fan relay
  } else {
    digitalWrite(FAN_RELAY_PIN, LOW);  // Turn off the fan relay
  }

  // Delay to avoid too frequent updates
  delay(2000); // Update every 2 seconds
}