#include <DHTSensor.h>

#define DHTPIN D4     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

void dhtCallback(float humidity, float temperature, float heatIndex);

DHTSensor dht(DHTPIN, DHTTYPE, *dhtCallback);

uint32_t elapsed = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  dht.setup();
}

void loop() {

  dht.loop();

  if ((millis() - elapsed) >= 15000) {
    elapsed = millis();
    Serial.println("Heartbeat");
  }
}

void dhtCallback(float humidity, float temperature, float heatIndex)
{

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C\t");
  Serial.print("Heat: ");
  Serial.println(heatIndex);

}
