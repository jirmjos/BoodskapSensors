#include "DHTSensor.h"

DHTSensor::DHTSensor(uint8_t pin, uint8_t type, void (*callback)(float humidity, float temperature, float heatIndex))
  : _dht(pin, type), _callback(callback), _oH(0.0), _oT(0.0), _oHic(0.0)
{

}

void DHTSensor::setup()
{

}

void DHTSensor::loop()
{

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = _dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = _dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = _dht.computeHeatIndex(t, h, false);

  if(h != _oH || t != _oT || hic != _oHic){
    _callback(h, t, hic);
  }

  _oH = h;
  _oT = t;
  _oHic = hic;

}
