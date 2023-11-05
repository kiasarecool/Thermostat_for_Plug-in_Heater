#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
// Custom I2C pins
#define BME_SDA 8  // Replace with the pin number you want to use for SDA
#define BME_SCL 9  // Replace with the pin number you want to use for SCL

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_SH1106 display; // Instantiate the display object without a reset pin
Adafruit_BME280 bme; // I2C

const char* ssid = "Your SSID";
const char* password = "Your PASSWORD";
// Relay and Button Pins
const int relayPin = 13;
const int setPointIncreaseButtonPin = 14;
const int setPointDecreaseButtonPin = 12;
const int setPointChangeButtonPin = 10;
const int ledOnPin = 17; //  GPIO pin for the "relay on" LED
const int ledOffPin = 18; //  GPIO pin for the "relay off" LED

// Temperature Setpoint
float temperatureSetpointF = 69.0; // Default setpoint
const float temperatureHysteresisF = 1.5;

// Debouncing Variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Display Blink Variables
unsigned long lastBlinkTime = 0;
const long blinkInterval = 500; // 500ms interval for blinking
bool displayOn = true;

// Flags
bool inSetpointMode = false;

void controlRelay(float currentTemperatureF);
void updateDisplay(float setpoint, float currentTemp, bool displayOn);

void setup() {
  Serial.begin(115200);

  Serial.println("Serial started");

  Wire.begin(BME_SDA, BME_SCL);

  if (!bme.begin(0x76)) { // Address for BME280
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  display.begin(SH1106_SWITCHCAPVCC, 0x3C); // Address for SH1106

  // The display.begin() does not return a value, so no need to check it
  display.clearDisplay(); // Clear the display buffer
  display.display(); // Show the initial display buffer
  delay(1000); // Pause for display to show the cleared buffer

  // Initialize relay and button pins
  pinMode(relayPin, OUTPUT);
  pinMode(setPointIncreaseButtonPin, INPUT_PULLUP);
  pinMode(setPointDecreaseButtonPin, INPUT_PULLUP);
  pinMode(setPointChangeButtonPin, INPUT_PULLUP);
  pinMode(ledOnPin, OUTPUT);
  pinMode(ledOffPin, OUTPUT);

  // Set the initial state for the relay and LEDs
  digitalWrite(relayPin, LOW); // Relay is off on startup
  digitalWrite(ledOnPin, LOW); // "Relay on" LED is off
  digitalWrite(ledOffPin, HIGH); // "Relay off" LED is on

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the OTA service
  ArduinoOTA.setHostname("ESP32-S2-THERMO-OTA");
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}



void loop() {
  unsigned long currentMillis = millis();

  // Check if the setpoint change button is being held down
  if (digitalRead(setPointChangeButtonPin) == LOW) {
    // Enter setpoint change mode
    inSetpointMode = true;

    // Check for increase button
    if (digitalRead(setPointIncreaseButtonPin) == LOW) {
      temperatureSetpointF += 0.50;
    }
    // Check for decrease button
    if (digitalRead(setPointDecreaseButtonPin) == LOW) {
      temperatureSetpointF -= 0.50;
    }
    
    updateDisplay(temperatureSetpointF, bme.readTemperature() * 1.8 + 32, true); // Update display with setpoint and current temp
  } else {
    // Exit setpoint change mode
    inSetpointMode = false;
    // Display the current temp and setpoint
    float temperatureF = bme.readTemperature() * 1.8 + 32; // Convert Celsius to Fahrenheit
    controlRelay(temperatureF);
    updateDisplay(temperatureSetpointF, temperatureF, true); // Update display with current temp and setpoint
  }

  ArduinoOTA.handle(); // Must be called in loop() for OTA to function
}

void controlRelay(float currentTemperatureF) {
  // If the current temperature is higher than the setpoint plus hysteresis,
  // turn the relay (and "relay on" LED) on. Otherwise, turn it off.
  if (currentTemperatureF > (temperatureSetpointF + temperatureHysteresisF)) {
    digitalWrite(relayPin, LOW); // Turn off relay
    digitalWrite(ledOnPin, HIGH); // Turn on "relay on" LED
    digitalWrite(ledOffPin, LOW); // Turn off "relay off" LED
  } else if (currentTemperatureF < (temperatureSetpointF - temperatureHysteresisF)) {
    digitalWrite(relayPin, HIGH); // Turn on relay
    digitalWrite(ledOnPin, LOW); // Turn off "relay on" LED
    digitalWrite(ledOffPin, HIGH); // Turn on "relay off" LED
  }
}


void updateDisplay(float setpoint, float currentTemp, bool displayOn) {
  display.clearDisplay();
  display.setTextSize(1); // Set text size to 1 for more text
  display.setTextColor(WHITE); // Set text color to white

  display.setCursor(0, 0); // Start at top-left corner
  display.print("Set: ");
  display.print(setpoint, 1);
  display.print(" F");

  display.setCursor(0, 10); // Move down a line
  display.print("Temp: ");
  display.print(currentTemp, 1);
  display.print(" F");

  // Read humidity
  float humidity = bme.readHumidity();
  display.setCursor(0, 20); // Move down another line
  display.print("Hum: ");
  display.print(humidity, 1);
  display.print("%");

  // Read pressure
  float pressure = bme.readPressure() / 100.0F; // Convert to hPa
  display.setCursor(0, 30); // Move down another line
  display.print("Pres: ");
  display.print(pressure, 1);
  display.print(" hPa");

  display.display(); // Update the display with all the new info
}



