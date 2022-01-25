#include <ESP8266WiFi.h>
#include <DHTesp.h>
#include <ThingSpeak.h>
#include "device.h"
#include "wifi_id.h"
#include "thingSpeak_id.h"

WiFiClient wifiClient;
void WifiConnect();
DHTesp dht;

void setup() {
	Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  dht.setup(PIN_DHT, DHTesp::DHT11);
  Serial.println("Booting...");
  WifiConnect();
  Serial.println("Connecting to ThingSpeak ...");
  ThingSpeak.begin(wifiClient);
}

void loop() {
    float h = dht.getHumidity();
    float t = dht.getTemperature();
    if (dht.getStatus() == DHTesp::ERROR_NONE)
    {
      Serial.printf("Temp: %.2f, Humidity: %.2f\n", t, h);
      ThingSpeak.setField(1, t);
      ThingSpeak.setField(2, h);
      int nResult = ThingSpeak.writeFields(SECRET_CH_ID, SECRET_WRITE_APIKEY);
      Serial.printf("ThingSpeak update channel result: %d\n", nResult);
      delay(20000);
    }
    else
      delay(1000);
}

void WifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.print("System connected with IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
}
