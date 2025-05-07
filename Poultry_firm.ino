#include <WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi credentials
const char* ssid = "Me";
const char* password = "mehedi113";

// ThingSpeak settings
const char* thingSpeakApiKey = "WQ38HT9QZWRE4FFX";
unsigned long channelNumber = 2948761;

// Sensor pins
const int mq7Pin = 34;
const int mq135Pin = 35;
const int dhtPin = 4;
#define DHTTYPE DHT22

// Output pins
const int statusLed = 12;
const int relayPin = 14;

// LCD setup (use Frank de Brabander's library)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT sensor
DHT dht(dhtPin, DHTTYPE);

// WiFi client
WiFiClient client;

// Threshold for MQ135
const int mq135Threshold = 300;

// Number of samples for averaging
const int numSamples = 10;

void connectToWiFi() {
  WiFi.begin(ssid, password);
  int attempts = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    lcd.print(".");
    attempts++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    for (int i = 0; i < 3; i++) {
      digitalWrite(statusLed, HIGH);
      delay(100);
      digitalWrite(statusLed, LOW);
      delay(100);
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed");
  }

  delay(1000);
}

int readAveragedAnalog(int pin) {
  long sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(pin);
    delay(10);  // short delay for stabilization
  }
  return sum / numSamples;
}

void setup() {
  Serial.begin(115200);
  pinMode(statusLed, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  lcd.init();         // Only works with Frank de Brabander's library
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Poultry Monitor");

  dht.begin();
  connectToWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int mq7Value = readAveragedAnalog(mq7Pin);
  int mq135Value = readAveragedAnalog(mq135Pin);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT!");
    temperature = 0;
    humidity = 0;
  }

  Serial.printf("Temp: %.1fC, Hum: %.1f%%\n", temperature, humidity);
  Serial.printf("MQ7: %d, MQ135: %d\n", mq7Value, mq135Value);

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print(" H:");
  lcd.print((int)humidity);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("M7:");
  lcd.print(mq7Value);
  lcd.print(" M135:");
  lcd.print(mq135Value);

  // Upload to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, mq7Value);
  ThingSpeak.setField(4, mq135Value);

  int response = ThingSpeak.writeFields(channelNumber, thingSpeakApiKey);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak.");
    digitalWrite(statusLed, HIGH);
    delay(200);
    digitalWrite(statusLed, LOW);
  } else {
    Serial.printf("Failed to send data. Code: %d\n", response);
  }

  // Relay logic
  if (mq135Value < mq135Threshold) {
    digitalWrite(relayPin, HIGH);
    delay(5000);
    digitalWrite(relayPin, LOW);
  }

  delay(15000);  // ThingSpeak rate limit
}
