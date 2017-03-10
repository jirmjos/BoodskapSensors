#ifndef _DHT_SENSOR_H
#define _DHT_SENSOR_H

#include <Arduino.h>
#include <SensorModule.h>
#include <DHT.h>

class DHTSensor : public SensorModule
{

  public:

    DHTSensor(uint8_t pin, uint8_t type, void (*callback)(float humidity, float temperature, float heatIndex));

    virtual void setup();

    virtual void loop();

  private:
    DHT _dht;
    void (*_callback)(float humidity, float temperature, float heatIndex);
    float _oH;
    float _oT;
    float _oHic;
};

#endif //_DHT_SENSOR_H
