
#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHTesp.h>

const char* Wifi_Name = "Wokwi-GUEST";
const char* Wifi_Pass = "";

const unsigned long channelNumber = 3549939;
const char* apiKey = "BN421U2HIFGD31JJ";

const char* universityID = "12216933";

const int DHT_PIN = 15;
DHTesp dht;
WiFiClient clientObject;

void setup() {
  Serial.begin(115200);

  dht.setup(DHT_PIN, DHTesp::DHT22);
  WiFi.mode(WIFI_STA);
  WiFi.begin(Wifi_Name, Wifi_Pass);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  ThingSpeak.begin(clientObject);

  Serial.print("University ID: ");
  Serial.println(universityID);
}

void loop() {
  TempAndHumidity data = dht.getTempAndHumidity();

  float temperature = data.temperature;
  float humidity = data.humidity;

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT22!");
    delay(20000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int responseCode =
      ThingSpeak.writeFields(channelNumber, apiKey);

  if (responseCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.print("Error sending data. HTTP code: ");
    Serial.println(responseCode);
  }

  delay(20000);
}
